#include "sensor_config.h"

static uint8_t count = 500;

uint32_t sens_val_conv__to_cm(void) {
  uint32_t converted_val = 0;
  uint32_t acc_sens_val = 0;
  while (count--) {
    acc_sens_val += sensor_config__read_raw_val();
  }
  count = 500;
  acc_sens_val /= count;

  // maximum range of sensor is 500cm.
  // maximum ADC value is 4096

  // if(acc_sens_val > 300) {
  converted_val = (acc_sens_val * 500) / 4096;
  //}
  // else {
  //     converted_val =
  // }

  return converted_val;
}