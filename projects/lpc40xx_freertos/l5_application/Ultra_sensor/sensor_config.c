#include "adc.h"
#include "gpio.h"
#include "lpc40xx.h"

static uint8_t sensor_adc_channel = ADC__CHANNEL_2;

void sensor_config__init(void) {

  adc__initialize();

  // Make Port 0 Pin 25 functionality as ADC0[2]
  gpio__construct_with_function(0, 25, GPIO__FUNCTION_1);

  // Set mode and analog input bits in control register
  LPC_IOCON->P0_25 &= ~(0b11 << 3);

  // Analog mode
  LPC_IOCON->P0_25 &= ~(1 << 7);

  LPC_IOCON->P0_25 &= ~(3 << 3);
}

uint32_t sensor_config__read_raw_val(void) { adc__get_adc_value(sensor_adc_channel); }