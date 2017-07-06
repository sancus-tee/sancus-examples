#ifndef _CONFIG_H


/* --- Debug Flags -------------------------------------------------------- */

//#define __DEBUG
#define __TIMING

#ifdef __TIMING
  #include <sancus_support/tsc.h>

  #define __TIMING_INIT();  \
    static uint64_t tsc0_, tsc1_, tscd_; \
    tsc0_ = tsc_read();

  #define __TIMING_REINIT();  \
    tsc0_ = tsc_read();

  #define __TIMING_PRINT(msg_); \
    tsc1_ = tsc_read(); \
    tscd_ = tsc1_ - tsc0_; \
    puts_n  (msg_); \
    puthex  ((uint8_t *) &tscd_, 8); \
    putchar ('\n'); \
    printf1 ("     == %u\n", (void *) *((unsigned int*) (&tscd_)));

#else
  #define __TIMING_INIT();
  #define __TIMING_REINIT();
  #define __TIMING_PRINT(msg_);
#endif


#ifdef __DEBUG
  #ifndef __DEBUG_PRINTF_WRAPPERS
    #define __DEBUG_PRINTF_WRAPPERS
  #endif
#endif


/* --- Crypto Configuration ----------------------------------------------- */

#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

#ifdef MSP430
  #define __SANCUS_MSP430 
#else
  #define __NOSANCUS 
#endif

#ifdef __SANCUS_MSP430
  #include <msp430.h>
  #include <sancus/sm_support.h>
  #undef dint
  #define dint()  asm ("dint")
  #undef eint
  #define eint()  asm ("eint")
#else
  #error "Not building for a Sancus-enabled CPU: unsupported."
#endif

#define HASH_TYPE_SPONGEWRAP  1

#ifdef __SANCUS_MSP430
  #define HASH_UINT32 unsigned long
  #define HASH_TYPE   HASH_TYPE_SPONGEWRAP
  #define HASH_SIZE   8
#endif

#ifndef HASH_TYPE
  #error "We don't have a hash implementation for this architecture."
#endif


/* --- TA Configuration --------------------------------------------------- */

#define MAX_INVARS    10
#define MAX_PROCESSES 10
#define NONCE_SIZE    HASH_SIZE


/* --- Timers, Intervals and Repetitions ---------------------------------- */

#ifdef __SANCUS_MSP430
  #ifdef __SANCUS_LOAD
    #define CLOCK_TA    5
  #endif
  #ifdef __SANCUS_SIM
    #define CLOCK_TA    0.5
  #endif
#endif

#ifdef __SANCUS_MSP430
  #ifdef __SANCUS_LOAD
    #define CLOCK_DIV   2
  #endif
  #ifdef __SANCUS_SIM
    #define CLOCK_DIV   1
  #endif
#endif

#ifdef __DEBUG
  #ifdef __SANCUS_SIM
    #define ROUNDS     50
  #endif
  #ifndef ROUNDS
    #define ROUNDS    500
  #endif
#endif


#define _CONFIG_H
#endif

