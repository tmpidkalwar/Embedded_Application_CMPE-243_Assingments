#pragma once

#include <stdio.h>

typedef struct gps_t {
  float latitude;
  float longitude;
} gps_t;

int get_matching_index(const gps_t **gps_array, size_t gps_array_size, const gps_t *match_to_find);