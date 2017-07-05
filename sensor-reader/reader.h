#ifndef READER_H
#define READER_H

#include <sancus/sm_support.h>
#include "sensor.h"

extern struct SancusModule reader;

typedef unsigned nonce_data;

typedef struct
{
    nonce_data nonce;
    sensor_data cipher;
    char tag[32];
} ReaderOutput;

void SM_ENTRY(reader) get_readings(ReaderOutput* out);

#endif

