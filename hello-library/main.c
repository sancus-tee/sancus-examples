#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>
#include "hello.h"

/* ======== UNTRUSTED CONTEXT ======== */
int main()
{   
    // Disable watchdog
    WDTCTL = WDTPW | WDTHOLD;
    msp430_io_init();
    puts("Testing untrusted library call..");
    untrusted_hello();
    puts("Testing trusted library call..");
    hello_greet();
    puts("All good.");

    hello_exit();
    ASSERT(0 && "should never reach here");
}

