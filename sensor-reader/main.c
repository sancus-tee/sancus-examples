#include <msp430.h>
#include <stdio.h>
#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>
#include "reader.h"

int main()
{
    msp430_io_init();

    pr_info("enabling sensor/reader SMs..");
    sancus_enable_info(&sensor);
    sancus_enable_info(&reader);

    pr_info("requesting sensor readings..");
    nonce_t no = 0xabcd;
    ReaderOutput out;
    get_readings(no, &out);

    pr_info("dumping sealed output from reader SM..");
    dump_buf((char*)&no, sizeof(no), "  Nonce");
    dump_buf((char*)&out.cipher, sizeof(out.cipher), "  Cipher");
    dump_buf((char*)&out.tag, sizeof(out.tag), "  Tag");

    pr_info("all done!");
    EXIT();
}
