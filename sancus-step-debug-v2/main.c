#include <msp430.h>
#include <stdio.h>
#include <sancus/sm_support.h>
#include <sancus_support/timer.h>
#include <sancus_support/sm_io.h>
#include <sancus_support/sancus_step.h>

/* ======== SANCUS STEP DBG CONSTANTS ======== */

#define RETI_LENGTH 0x5
#define SS_DBG_RECORD_DELAY 0x8

/* ======== UNTRUSTED CONTEXT ======== */

extern int ss_dbg_entry_delay;
extern int ss_dbg_interrupted;

/**
 * Returns the latency of resuming an interrupted module,
 * excluding any other instructions, i.e. the latency of a reti call
 * executed from within the isr
 */
int get_isr_latency(void)
{
    timer_tsc_start();
    int isr_latency_t = ss_dbg_get_isr_latency();
    timer_irq(ss_dbg_entry_delay);
    isr_latency_t = ss_dbg_get_isr_latency();
    /* amount of cycles in the reti logic = measured delay
                                  - record delay
                                  - duration of reti instruction
                                  + 1 (because we count amount of cycles)
    */
    return isr_latency_t - SS_DBG_RECORD_DELAY - RETI_LENGTH + 1;
}

int main()
{
    msp430_io_init();
    sancus_enable(&ssdbg);
    
    isr_latency = get_isr_latency();
    printf("isr latency: %d\n", isr_latency);
    printf("all done!\n");
    EXIT();
}

/* ======== TIMER A ISR ======== */

extern uint16_t __ss_isr_stack[];
extern void*    __ss_isr_sp;

__attribute__((naked)) __attribute__((interrupt(TIMER_IRQ_VECTOR)))
void timerA_isr_entry(void)
{
    __asm__("cmp #0x0, r1\n\t"
            "jne 1f\n\t"
            "; sm got interrupted\n\t"
            "mov #0x1, %0\n\t"
            "mov &__ss_isr_sp, r1\n\t"
            "push r15\n\t"
            "push #0x0\n\t"
            "mov #4, &%1\n\t"
            "mov #0x220, &%1\n\t"
            "1:\n\t"
            "; no sm interrupted\n\t"
            "reti\n\t"
            ::"m"(ss_dbg_interrupted), "m"(TACTL):);
}

