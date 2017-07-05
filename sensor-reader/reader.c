#include "reader.h"

#include <stdio.h>

DECLARE_SM(reader, 0x1234);

static sensor_data SM_FUNC(reader) transform_readings(sensor_data data)
{
    return data ^ 0xcafe;
}

void get_readings(ReaderOutput* out)
{
    sensor_data data = transform_readings(read_sensor_data());
    out->nonce = 0xbabe;
    int res = sancus_wrap(&out->nonce, sizeof(out->nonce),
                          &data, sizeof(data),
                          &out->cipher, out->tag);

    if (!res)
        puts("Wrapping failed");
}
