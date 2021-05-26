#include "solution.h"

void print_gps_t(const gps_t *gps_param) {
  if (gps_param == NULL) {
    return;
  }
  fprintf(stderr, "Received GPS parameters as latitude = %f, longitude = %f\n", gps_param->latitude,
          gps_param->longitude);
}

int get_matching_index(const gps_t **gps_array, size_t gps_array_size, const gps_t *match_to_find) {
  if (gps_array == NULL || match_to_find == NULL) {
    return -2;
  }
  for (size_t i = 0; i < gps_array_size; i++) {
    if ((gps_array[i]->latitude == match_to_find->latitude) && (gps_array[i]->longitude == match_to_find->longitude)) {
      return i;
    }
    if (gps_array[i] == NULL) {
      return -2;
    }
  }
  return -1;
}