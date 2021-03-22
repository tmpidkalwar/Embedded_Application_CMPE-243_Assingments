#include "can_handle_message_rx.h"
#include "board_io.h"
#include "can_bus.h"
#include "driver_logic.h"
#include "gpio.h"
#include "project.h"
#include <stdio.h>

const dbc_SENSOR_SONARS_s dbc_mia_replacement_SENSOR_SONARS = {};
const uint32_t dbc_mia_threshold_SENSOR_SONARS = SENSOR_SONARS_MIA_THRESHOLD;
static dbc_SENSOR_SONARS_s sensor_data = {};

void handle_all_can_messages_rx(can__num_e can_num) {
  can__msg_t can_msg = {};
  int timeout = 0;
  gpio_s sensor_mia_led_gpio = board_io__get_led0();
  static int cnt = 0;
  while (can__rx(can_num, &can_msg, timeout)) {
    const dbc_message_header_t header = {.message_id = can_msg.msg_id, .message_dlc = can_msg.frame_fields.data_len};

    if (dbc_decode_SENSOR_SONARS(&sensor_data, header, can_msg.data.bytes)) {
      driver_process_input(&sensor_data);
    }
    cnt++;
    printf("Here cnt = %d\n", cnt);
  }

  if (dbc_service_mia_SENSOR_SONARS(&sensor_data, 1)) {
    // In this example, setting the led0 indicates MIA
    //  printf("sensor data count = %d\n", sensor_data.mia_info.mia_counter);
    gpio__set(sensor_mia_led_gpio);
    printf("MIA hit\n");
  } else {
    // Reset the mia_led
    gpio__reset(sensor_mia_led_gpio);
  }
  printf("sensor data count = %d\n", sensor_data.mia_info.mia_counter);
}
