#ifndef READER_H
#define READER_H

#include <sancus/sm_support.h>

#include "sensor.h"

extern struct SancusModule reader;

typedef struct
{
    char cipher[sizeof(sensor_data_t)];
    char tag[SANCUS_TAG_SIZE];
} ReaderOutput;

typedef unsigned nonce_t;

void SM_ENTRY(reader) get_readings(nonce_t no, ReaderOutput* out);

#endif
