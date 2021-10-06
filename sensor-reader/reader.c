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

    /* Assert untrusted code loaded SMs in the correct order */
    ASSERT(sancus_get_caller_id() == SM_ID_UNPROTECTED);
    ASSERT(sancus_get_self_id() == 2);
    ASSERT(sancus_get_id((void*) read_sensor_data) == 1);

    /* Securely verify and call sensor SM. */
    sensor_data_t data = read_sensor_data();
    //TODO caller_id should remain 0 after merging Aion compiler changes
    ASSERT(sancus_get_caller_id() == 1);
    dump_buf((uint8_t*)&data, sizeof(sensor_data_t), "  Data");
    ASSERT(sancus_get_caller_id() == SM_ID_UNPROTECTED);

    /* Transform and seal sensor readings. */
    sensor_data_t transformed = transform_readings(data);
    ASSERT(sancus_wrap(&no, sizeof(no), &transformed, sizeof(transformed),
                       &out->cipher, &out->tag));
}
