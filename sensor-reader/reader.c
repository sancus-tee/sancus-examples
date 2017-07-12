#include "reader.h"

#include <stdio.h>

DECLARE_SM(reader, 0x1234);

static sensor_data SM_FUNC(reader) transform_readings(sensor_data data)
{
    return data ^ 0xcafe;
}

void SM_ENTRY(reader) get_readings(nonce no, ReaderOutput* out)
{
    sensor_data data = read_sensor_data();
    sensor_data transformed = transform_readings(data);

    if (!sancus_wrap(&no, sizeof(no), &transformed, sizeof(transformed),
                     out->cipher, out->tag))
    {
        puts("Error encrypting data");
    }
}
