#include "reader.h"
#include <sancus/sm_support.h>
#include "../common.h"

#include <stdio.h>

DECLARE_SM(reader, 0x1234);

static sensor_data SM_FUNC(reader) transform_readings(sensor_data data)
{
    return data ^ 0xcafe;
}

int SM_FUNC(reader) outside_sm(void *p)
{
    return ( (p < (void*) &__PS(reader)) || (p >= (void*) &__PE(reader)) ) &&
           ( (p < (void*) &__SS(reader)) || (p >= (void*) &__SE(reader)) );
}

void SM_ENTRY(reader) get_readings(nonce no, ReaderOutput* out)
{
    /* To avoid TOCTOU vulnerabilities, we first copy the unprotected cipher
       and tag pointers within the SM + check 'out' before dereferencing. */
    ASSERT(outside_sm(out));
    char *cipher = out->cipher;
    char *tag = out->tag;

    /* Now check whether output memory ranges are indeed outside SM. */
    ASSERT(outside_sm(cipher) && outside_sm(cipher + sizeof(sensor_data) - 1));
    ASSERT(outside_sm(tag) && outside_sm(tag + SANCUS_TAG_SIZE - 1));

    /* Securely verify and call sensor SM. */
    sensor_data data = read_sensor_data();

    /* Transform and encrypt sensor readings. */
    sensor_data transformed = transform_readings(data);
    ASSERT(sancus_wrap(&no, sizeof(no), &transformed, sizeof(transformed),
                       cipher, tag));
}
