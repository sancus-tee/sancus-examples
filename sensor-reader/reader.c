#include "reader.h"
#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>

DECLARE_SM(reader, 0x1234);

sensor_data_t SM_FUNC(reader) transform_readings(sensor_data_t data)
{
    return data;
}

void SM_ENTRY(reader) get_readings(nonce_t no, ReaderOutput* out)
{
    /* Ensure output memory range about to be dereferenced lies outside SM. */
    ASSERT(sancus_is_outside_sm(reader, out, sizeof(ReaderOutput)));

    /* Securely verify and call sensor SM. */
    sensor_data_t data = read_sensor_data();
    dump_buf((uint8_t*)&data, sizeof(sensor_data_t), "  Data");

    /* Transform and seal sensor readings. */
    sensor_data_t transformed = transform_readings(data);
    ASSERT(sancus_wrap(&no, sizeof(no), &transformed, sizeof(transformed),
                       &out->cipher, &out->tag));
}
