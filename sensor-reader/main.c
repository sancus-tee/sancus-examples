#include <msp430.h>
#include <stdio.h>
#include <sancus/sm_support.h>
#include "reader.h"
#include "../common.h"

int main()
{
    msp430_init();

    pr_info("enabling sensor/reader SMs..");
    do_sancus_enable(&sensor);
    do_sancus_enable(&reader);

    pr_info("requesting sensor readings..");
    nonce no = 0xabcd;
    ReaderOutput out;
    get_readings(no, &out);

    dump_buf((char*)&no, sizeof(no), "Nonce");
    dump_buf((char*)&out.cipher, sizeof(out.cipher), "Cipher");
    dump_buf((char*)&out.tag, sizeof(out.tag), "Tag");

    pr_info("all done!");
    return 0;
}
