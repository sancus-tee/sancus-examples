#include <msp430.h>
#include <stdio.h>
#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>
#include "reader.h"

int main()
{
    msp430_io_init();

    pr_info("enabling sensor/reader SMs..");
    sancus_enable(&reader);
    pr_sm_info(&reader);
    sancus_enable(&sensor);
    pr_sm_info(&sensor);

    pr_info("requesting sensor readings..");
    nonce_t no = 0xabcd;
    ReaderOutput out;
    ASSERT(sancus_get_caller_id() == SM_ID_IRQ);
    get_readings(no, &out);
    ASSERT(sancus_get_caller_id() == 1);

    pr_info("dumping sealed output from reader SM..");
    dump_buf((uint8_t*)&no, sizeof(no), "  Nonce");
    dump_buf((uint8_t*)&out.cipher, sizeof(out.cipher), "  Cipher");
    dump_buf((uint8_t*)&out.tag, sizeof(out.tag), "  Tag");

    reader_exit();
    ASSERT(0 && "should never reach here");
}
