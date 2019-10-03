#include <string.h> // memcpy

#include "i2c.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "clock.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"
#include "sys_time.h"

/// Set to non-zero to enable debugging
#define DEBUG_I2C 0

#if DEBUG_I2C
#include <stdio.h>
#define I2C__DEBUG_PRINTF(f_, ...)                                                                                     \
  do {                                                                                                                 \
    fprintf(stderr, "I2C:");                                                                                           \
    fprintf(stderr, (f_), ##__VA_ARGS__);                                                                              \
    fprintf(stderr, "\n");                                                                                             \
  } while (0)
#else
#define I2C__DEBUG_PRINTF(f_, ...) /* NOOP */
#endif

/**
 * Data structure for each I2C peripheral
 */
typedef struct {
  lpc_peripheral_e peripheral_id;   ///< Indicates which peripheral (I2C0, I2C1 etc.) this struct is for
  LPC_I2C_TypeDef *const registers; ///< LPC memory mapped registers for the I2C bus

  // Transfer complete signal informs us when I2C state machine driven by ISR has finished
  StaticSemaphore_t transfer_complete_signal_memory;
  SemaphoreHandle_t transfer_complete_signal;

  // Mutex to ensure only one transaction is performed at a time
  StaticSemaphore_t mutex_memory;
  SemaphoreHandle_t mutex;

  // These are the parameters we save before a transaction is started
  uint8_t error_code;
  uint8_t device_address;
  uint8_t first_register;
  size_t bytes_to_transfer;
  uint8_t *user_data;
} i2c_s;

/// I2C state machine runs as long as return code is busy
typedef enum {
  I2C__BUSY = 0,
  I2C__STOP_SENT,
} i2c__state_e;

/// Instances of structs for each I2C peripheral
static i2c_s i2c_structs[] = {
    {LPC_PERIPHERAL__I2C0, LPC_I2C0},
    {LPC_PERIPHERAL__I2C1, LPC_I2C1},
    {LPC_PERIPHERAL__I2C2, LPC_I2C2},
};

static bool i2c__transfer(i2c_s *i2c, uint8_t device_address, uint8_t first_register, uint8_t *data,
                          uint32_t number_of_bytes);
static void i2c__kick_off_transfer(i2c_s *i2c, uint8_t device_address, uint8_t first_register, uint8_t *data,
                                   uint32_t number_of_bytes);
static i2c__state_e i2c__handle_state_machine(i2c_s *i2c);
static void i2c__set_stop(LPC_I2C_TypeDef *i2c);
static void i2c__handle_interrupt(i2c_s *i2c);

/**
 * Instead of using a dedicated variable for read vs. write, we just use the LSB of
 * the user address to indicate read or write mode.
 */
static void i2c__flag_read_mode(uint8_t *device_addr) { *device_addr |= 0x01; }
static void i2c__flag_write_mode(uint8_t *device_addr) { *device_addr &= ~0x01; }
static uint8_t i2c__read_address(uint8_t device_addr) { return device_addr | 0x01; }
static uint8_t i2c__write_address(uint8_t device_addr) { return (device_addr & 0xFE); }
static bool i2c__is_read_address(uint8_t device_addr) { return 0 != (device_addr & 0x01); }

/**
 * CONSET and CONCLR register bits
 * 0x04 AA
 * 0x08 SI
 * 0x10 STOP
 * 0x20 START
 * 0x40 ENABLE
 */
void i2c__clear_si_flag(LPC_I2C_TypeDef *i2c) { i2c->CONCLR = 0x08; }
void i2c__set_start_flag(LPC_I2C_TypeDef *i2c) { i2c->CONSET = 0x20; }
void i2c__clear_start_flag(LPC_I2C_TypeDef *i2c) { i2c->CONCLR = 0x20; }
void i2c__set_ack_flag(LPC_I2C_TypeDef *i2c) { i2c->CONSET = 0x04; }
void i2c__set_nack_flag(LPC_I2C_TypeDef *i2c) { i2c->CONCLR = 0x04; }

static void i2c0_isr(void) { i2c__handle_interrupt(&i2c_structs[I2C__0]); }
static void i2c1_isr(void) { i2c__handle_interrupt(&i2c_structs[I2C__1]); }
static void i2c2_isr(void) { i2c__handle_interrupt(&i2c_structs[I2C__2]); }

/*******************************************************************************
 *
 *                      P U B L I C    F U N C T I O N S
 *
 ******************************************************************************/

void i2c__initialize(i2c_e i2c_number, uint32_t bus_speed_in_khz) {
  i2c_s *i2c = &i2c_structs[i2c_number];
  const function__void_f isrs[] = {i2c0_isr, i2c1_isr, i2c2_isr};

  // Create binary semaphore and mutex
  i2c->transfer_complete_signal = xSemaphoreCreateBinaryStatic(&(i2c->transfer_complete_signal_memory));
  i2c->mutex = xSemaphoreCreateMutexStatic(&(i2c->mutex_memory));

  // Optional: Provide names of the FreeRTOS objects for the Trace Facility
  // vTraceSetMutexName(mI2CMutex, "I2C Mutex");
  // vTraceSetSemaphoreName(mTransferCompleteSignal, "I2C Finish Sem");

  lpc_peripheral__turn_on_power_to(i2c->peripheral_id);
  i2c->registers->CONCLR = 0x6C; // Clear ALL I2C Flags

  /**
   * Per I2C high speed mode:
   * HS mode master devices generate a serial clock signal with a HIGH to LOW ratio of 1 to 2.
   * So to be able to optimize speed, we use different duty cycle for high/low
   *
   * Compute the I2C clock dividers.
   * The LOW period can be longer than the HIGH period because the rise time
   * of SDA/SCL is an RC curve, whereas the fall time is a sharper curve.
   */
  const uint32_t percent_high = 40;
  const uint32_t percent_low = (100 - percent_high);
  const uint32_t freq_hz = (bus_speed_in_khz > 1000) ? (100 * 1000) : (bus_speed_in_khz * 1000);
  const uint32_t half_clock_divider = (clock__get_peripheral_clock_hz() / freq_hz) / 2;
  i2c->registers->SCLH = (half_clock_divider * percent_high) / 100;
  i2c->registers->SCLL = (half_clock_divider * percent_low) / 100;

  // Set I2C slave address and enable I2C
  i2c->registers->ADR0 = 0;
  i2c->registers->ADR1 = 0;
  i2c->registers->ADR2 = 0;
  i2c->registers->ADR3 = 0;

  // Enable I2C and the interrupt for it
  i2c->registers->CONSET = 0x40;
  lpc_peripheral__enable_interrupt(i2c->peripheral_id, isrs[i2c_number]);
}

bool i2c__check_response(i2c_e i2c_number, uint8_t device_address) {
  // The I2C State machine will not continue after 1st state when length is set to 0
  const uint32_t zero_bytes = 0;
  const uint8_t dummy_register = 0;
  uint8_t unused = 0;

  return i2c__write_registers(i2c_number, device_address, dummy_register, &unused, zero_bytes);
}

bool i2c__read_registers(i2c_e i2c_number, uint8_t device_address, uint8_t first_register, uint8_t *bytes_to_read,
                         uint32_t number_of_bytes) {
  i2c__flag_read_mode(&device_address);
  return i2c__transfer(&i2c_structs[i2c_number], device_address, first_register, bytes_to_read, number_of_bytes);
}

uint8_t i2c__read_register(i2c_e i2c_number, uint8_t device_address, uint8_t register_address) {
  uint8_t byte = 0;
  i2c__read_registers(i2c_number, device_address, register_address, &byte, 1);
  return byte;
}

bool i2c__write_registers(i2c_e i2c_number, uint8_t device_address, uint8_t first_register, uint8_t *bytes_to_write,
                          uint32_t number_of_bytes) {
  i2c__flag_write_mode(&device_address);
  return i2c__transfer(&i2c_structs[i2c_number], device_address, first_register, bytes_to_write, number_of_bytes);
}

bool i2c__write_register(i2c_e i2c_number, uint8_t device_address, uint8_t register_address, uint8_t value) {
  return i2c__write_registers(i2c_number, device_address, register_address, &value, 1);
}

/*******************************************************************************
 *
 *                     P R I V A T E    F U N C T I O N S
 *
 ******************************************************************************/

static bool i2c__transfer(i2c_s *i2c, uint8_t device_address, uint8_t first_register, uint8_t *data,
                          uint32_t number_of_bytes) {
  bool status = false;
  const uint32_t timeout_ms = 3000;

  if (NULL == data) {
    return status;
  }

  // If scheduler not running, perform polling transaction
  if (taskSCHEDULER_RUNNING != xTaskGetSchedulerState()) {
    i2c__kick_off_transfer(i2c, device_address, first_register, data, number_of_bytes);

    // Wait for transfer to finish
    const uint64_t timeout = sys_time__get_uptime_ms() + timeout_ms;
    while (!xSemaphoreTake(i2c->transfer_complete_signal, 0)) {
      if (sys_time__get_uptime_ms() > timeout) {
        break;
      }
    }

    status = (0 == i2c->error_code) && (sys_time__get_uptime_ms() < timeout);
  }
  // FreeRTOS is running:
  else {
    xSemaphoreTake(i2c->mutex, portMAX_DELAY);
    {
      // Clear potential stale transfer complete signal
      xSemaphoreTake(i2c->transfer_complete_signal, 0);

      i2c__kick_off_transfer(i2c, device_address, first_register, data, number_of_bytes);

      // Wait for transfer to finish; the signal will be sent by the ISR once the transaction finishes
      if (xSemaphoreTake(i2c->transfer_complete_signal, timeout_ms)) {
        status = (0 == i2c->error_code);
      }
    }
    xSemaphoreGive(i2c->mutex);
  }

  I2C__DEBUG_PRINTF("  Finished with state: 0x%02X", (int)i2c->error_code);
  return status;
}

static void i2c__kick_off_transfer(i2c_s *i2c, uint8_t device_address, uint8_t first_register, uint8_t *data,
                                   uint32_t number_of_bytes) {
  i2c->error_code = 0;
  i2c->device_address = device_address;
  i2c->first_register = first_register;
  i2c->user_data = data;
  i2c->bytes_to_transfer = number_of_bytes;

  // Send START, I2C State Machine will finish the rest
  I2C__DEBUG_PRINTF("Starting transfer with device address: 0x%02X", (unsigned)device_address);
  i2c->registers->CONSET = 0x20;
}

static i2c__state_e i2c__handle_state_machine(i2c_s *i2c) {
  enum {
    // General states :
    I2C__STATE_BUS_ERROR = 0x00,
    I2C__STATE_START = 0x08,
    I2C__STATE_REPEAT_START = 0x10,
    I2C__STATE_ARBRITRATION_LOST = 0x38,

    // Master Transmitter States:
    I2C__STATE_MT_SLAVE_ADDR_ACK = 0x18,
    I2C__STATE_MT_SLAVE_ADDR_NACK = 0x20,
    I2C__STATE_MT_SLAVE_DATA_ACK = 0x28,
    I2C__STATE_MT_SLAVE_DATA_NACK = 0x30,

    // Master Receiver States:
    I2C__STATE_MR_SLAVE_READ_ACK = 0x40,
    I2C__STATE_MR_SLAVE_READ_NACK = 0x48,
    I2C__STATE_MR_SLAVE_ACK_SENT = 0x50,
    I2C__STATE_MR_SLAVE_NACK_SENT = 0x58,
  };

  i2c__state_e state = I2C__BUSY;

  /*
   ***********************************************************************************************************
   * Write-mode state transition :
   * I2C__STATE_START --> I2C__STATE_MT_SLAVE_ADDR_ACK --> I2C__STATE_MT_SLAVE_DATA_ACK -->
   *                                                  ... (I2C__STATE_MT_SLAVE_DATA_ACK) --> (stop)
   *
   * Read-mode state transition :
   * I2C__STATE_START --> I2C__STATE_MT_SLAVE_ADDR_ACK --> dataAcked --> I2C__STATE_REPEAT_START -->
   * I2C__STATE_MR_SLAVE_READ_ACK For 2+ bytes:  I2C__STATE_MR_SLAVE_ACK_SENT --> ... (I2C__STATE_MR_SLAVE_ACK_SENT) -->
   * I2C__STATE_MR_SLAVE_NACK_SENT --> (stop) For 1  byte :  I2C__STATE_MR_SLAVE_NACK_SENT --> (stop)
   ***********************************************************************************************************
   */

  LPC_I2C_TypeDef *i2c_reg = i2c->registers;
  const unsigned i2c_state = i2c_reg->STAT;
  I2C__DEBUG_PRINTF("  HW State: 0x%02X", i2c_state);

  switch (i2c_state) {
  // Start condition sent, so send the device address
  case I2C__STATE_START:
    i2c_reg->DAT = i2c__write_address(i2c->device_address);
    i2c__clear_si_flag(i2c_reg);
    break;

  case I2C__STATE_REPEAT_START:
    i2c_reg->DAT = i2c__read_address(i2c->device_address);
    i2c__clear_si_flag(i2c_reg);
    break;

  // Slave acknowledged its address, so send the first register
  case I2C__STATE_MT_SLAVE_ADDR_ACK:
    i2c__clear_start_flag(i2c_reg);

    // No data to transfer, this is used just to test if the slave responds
    if (0 == i2c->bytes_to_transfer) {
      i2c__set_stop(i2c_reg);
      state = I2C__STOP_SENT;
    } else {
      i2c_reg->DAT = i2c->first_register;
      i2c__clear_si_flag(i2c_reg);
    }
    break;

  // Slave acknowledged the data byte we sent, so send more bytes or finish the transaction by sending STOP
  case I2C__STATE_MT_SLAVE_DATA_ACK:
    if (i2c__is_read_address(i2c->device_address)) {
      i2c__set_start_flag(i2c_reg); // Send Repeat-start for read-mode
      i2c__clear_si_flag(i2c_reg);
    } else {
      if (0 == i2c->bytes_to_transfer) {
        i2c__set_stop(i2c_reg);
        state = I2C__STOP_SENT;
      } else {
        i2c_reg->DAT = *(i2c->user_data);
        ++(i2c->user_data);
        --(i2c->bytes_to_transfer);
        i2c__clear_si_flag(i2c_reg);
      }
    }
    break;

  /* In this state, we are about to initiate the transfer of data from slave to us
   * so we are just setting the ACK or NACK that we'll do AFTER the byte is received.
   */
  case I2C__STATE_MR_SLAVE_READ_ACK:
    i2c__clear_start_flag(i2c_reg);

    // 1+ bytes: Send ACK to receive a byte and transition to I2C__STATE_MR_SLAVE_ACK_SENT
    if (i2c->bytes_to_transfer > 1) {
      i2c__set_ack_flag(i2c_reg);
    }
    // 1 byte : NACK next byte to go to I2C__STATE_MR_SLAVE_NACK_SENT for 1-byte read
    else {
      i2c__set_nack_flag(i2c_reg);
    }

    i2c__clear_si_flag(i2c_reg);
    break;

  case I2C__STATE_MR_SLAVE_ACK_SENT:
    *(i2c->user_data) = i2c_reg->DAT;
    ++(i2c->user_data);
    --(i2c->bytes_to_transfer);

    if (1 == i2c->bytes_to_transfer) { // Only 1 more byte remaining
      i2c__set_nack_flag(i2c_reg);     // NACK next byte --> Next state: I2C__STATE_MR_SLAVE_NACK_SENT
    } else {
      i2c__set_ack_flag(i2c_reg); // ACK next byte --> Next state: I2C__STATE_MR_SLAVE_ACK_SENT(back to this state)
    }

    i2c__clear_si_flag(i2c_reg);
    break;

  case I2C__STATE_MR_SLAVE_NACK_SENT: // Read last-byte from Slave
    *(i2c->user_data) = i2c_reg->DAT;
    i2c__set_stop(i2c_reg);
    state = I2C__STOP_SENT;
    break;

  case I2C__STATE_ARBRITRATION_LOST:
    // We should not issue stop() in this condition, but we still need to end our  transaction.
    state = I2C__STOP_SENT;
    i2c->error_code = i2c_reg->STAT;
    break;

  case I2C__STATE_MT_SLAVE_ADDR_NACK: // no break
  case I2C__STATE_MT_SLAVE_DATA_NACK: // no break
  case I2C__STATE_MR_SLAVE_READ_NACK: // no break
  case I2C__STATE_BUS_ERROR:          // no break
  default:
    i2c->error_code = i2c_reg->STAT;
    i2c__set_stop(i2c_reg);
    state = I2C__STOP_SENT;
    break;
  }

  return state;
}

static void i2c__set_stop(LPC_I2C_TypeDef *i2c) {
  const uint32_t stop_bit = (1 << 4);
  i2c__clear_start_flag(i2c);
  i2c->CONSET = stop_bit;
  i2c__clear_si_flag(i2c);

  while (i2c->CONSET & stop_bit) {
    ;
  }
}

static void i2c__handle_interrupt(i2c_s *i2c) {
  /* If transfer finished (not busy), then give the signal */
  if (I2C__BUSY != i2c__handle_state_machine(i2c)) {
    long higher_priority_task_woke = 0;
    xSemaphoreGiveFromISR(i2c->transfer_complete_signal, &higher_priority_task_woke);
    portEND_SWITCHING_ISR(higher_priority_task_woke);
  }
}
