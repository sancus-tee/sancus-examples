#include "sensor.h"

DECLARE_MMIO_SM(sensor, /*[secret_start, end[=*/ TSC_BASE, TSC_BASE + TSC_SIZE,
                        /*vendor_id=*/ 0x1234);

sensor_data_t SM_MMIO_ENTRY(sensor) read_sensor_data(void)
{
    asm("mov.b #1, %0       \n\t"
        "mov.w %1+0, r12    \n\t"
        "mov.w %1+2, r13    \n\t"
        "mov.w %1+4, r14    \n\t"
        "mov.w %1+6, r15    \n\t"
    :: "m"(TSC_CTL), "m"(TSC_VAL):);
}
