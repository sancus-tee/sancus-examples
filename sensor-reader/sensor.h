#ifndef SENSOR_H
#define SENSOR_H

#include <sancus/sm_support.h>
#include <sancus_support/tsc.h>

extern struct SancusModule sensor;

typedef tsc_t sensor_data_t;

sensor_data_t SM_MMIO_ENTRY(sensor) read_sensor_data(void);

#endif
