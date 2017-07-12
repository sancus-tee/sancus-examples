#ifndef SENSOR_H
#define SENSOR_H

#include <sancus/sm_support.h>

extern struct SancusModule sensor;

typedef unsigned sensor_data;

sensor_data SM_ENTRY(sensor) read_sensor_data(void);

#endif
