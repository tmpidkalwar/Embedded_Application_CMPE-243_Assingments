#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * @file
 * This provides a simple I2C driver for all I2C peripherals
 *
 * Most typical implementation of an I2C device is that after the device address is sent, the first byte
 * is interpreted as a register number we wish to read or write. Devices that deviate from this
 * implementation may require enhancing this I2C driver.
 */

typedef enum {
  I2C__0,
  I2C__1,
  I2C__2,
} i2c_e;

/// I2C peripheral must be initialized before it is used
void i2c__initialize(i2c_e i2c_number, uint32_t bus_speed_in_khz);

/**
 * @returns true if the I2C device at the given address responds back with an ACK
 * @note this does not transfer any data
 */
bool i2c__check_response(i2c_e i2c_number, uint8_t device_address);

/**
 * @{
 * @name Reads one or more registers of a device_address
 * @param first_register This is the first byte sent on I2C after the device_address
 */
bool i2c__read_registers(i2c_e i2c_number, uint8_t device_address, uint8_t first_register, uint8_t *bytes_to_read,
                         uint32_t number_of_bytes);
uint8_t i2c__read_register(i2c_e i2c_number, uint8_t device_address, uint8_t register_address);
/** @} */

/**
 * @{
 * @name Writes one or more registers of a device_address
 * @param first_register This is the first byte sent on I2C after the device_address
 */
bool i2c__write_registers(i2c_e i2c_number, uint8_t device_address, uint8_t first_register, uint8_t *bytes_to_write,
                          uint32_t number_of_bytes);
bool i2c__write_register(i2c_e i2c_number, uint8_t device_address, uint8_t register_address, uint8_t value);
/** @} */
