// @file gps.h
#pragma once

// Notice the simplicity of this module. This module is easily mockable and provides a very
// simple API interface UART driver and line buffer module will be hidden inside of gps.c

void gps__run_once(void);
float gps__get_latitude(void);