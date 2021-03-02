// @file gps.h
#pragma once

// Notice the simplicity of this module. This module is easily mockable and provides a very
// simple API interface UART driver and line buffer module will be hidden inside of gps.c

typedef struct {
  float latitude;
  float longitude;
} gps_coordinates_t;

void gps__init(void);
void gps__run_once(void);

gps_coordinates_t gps__get_coordinates(void);

// void fake_gps__run_once(void);

// void fake_gps__init(void);