#include "sensor_node.h"

#include "board_io.h"
#include "uart_printf.h"
#include "ultrasonic.h"

#include "can_bus.h"
#include "tesla_model_rc.h"

#include "gpio.h"

static bool sensor_node__is_sync = false;
static dbc_DRIVER_HEARTBEAT_s can_msg__driver_heartbeat = {0};

static ultrasonic_s front_ultrasonic;

void sensor_node__init(void) {
  ultrasonic__initialize(&front_ultrasonic, GPIO__PORT_0, 7, GPIO__PORT_0, 6); // P0.7 Trigger, P0.6 Echo
}

static bool sensor_node__construct_and_send_fake_sonars(void) {
  dbc_SENSOR_SONARS_s sensor_sonar_struct = {0};

  sensor_sonar_struct.SENSOR_SONARS_LEFT = ultrasonic__get_fake_range();
  sensor_sonar_struct.SENSOR_SONARS_RIGHT = ultrasonic__get_fake_range();
  sensor_sonar_struct.SENSOR_SONARS_FRONT = ultrasonic__get_fake_range();
  sensor_sonar_struct.SENSOR_SONARS_BACK = ultrasonic__get_fake_range();

  return dbc_encode_and_send_SENSOR_SONARS(NULL, &sensor_sonar_struct);
}

bool sensor_node__send_messages_over_can(void) {
  bool sent_all_messages = false;

  if (sensor_node__is_sync) {
    sent_all_messages = sensor_node__construct_and_send_fake_sonars();
  }

  return sent_all_messages;
}

void sensor_node__handle_mia(void) {
  // We are in 10hz slot, so increment MIA counter by 100ms
  const uint32_t mia_increment_value = 100;

  if (dbc_service_mia_DRIVER_HEARTBEAT(&can_msg__driver_heartbeat, mia_increment_value)) {
    uart_printf(UART__0, "driver missing\r\n"); // use printf
    sensor_node__is_sync = false;
    gpio__set(board_io__get_led0());
  }
}

void sensor_node__handle_messages_over_can(void) {
  can__msg_t can_msg = {0};

  while (can__rx(can1, &can_msg, 0)) {
    // Construct "message header" that we need for the decode_*() API
    const dbc_message_header_t header = {
        .message_id = can_msg.msg_id,
        .message_dlc = can_msg.frame_fields.data_len,
    };

    // make this a seperate heartbeat handle message function
    if (dbc_decode_DRIVER_HEARTBEAT(&can_msg__driver_heartbeat, header, can_msg.data.bytes)) {
      if (!sensor_node__is_sync) {
        uart_printf(UART__0, "sensor sync\r\n"); // use printf
        sensor_node__is_sync = true;
        gpio__reset(board_io__get_led0());
      }
    }

  }
}

// remove when done testing
void sensor_node__print_ultrasonic_data(void) {
  uart_printf(UART__0, "distance: %u\r\n", (unsigned int)front_ultrasonic.distance); // use printf
}

// remove when done testing
void sensor_node__collect_ultrasonic_data(void) { ultrasonic__get_range(&front_ultrasonic); }
