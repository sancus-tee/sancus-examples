#include <stdint.h>
#include <msp430.h>
#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>
#include <sancus_support/sancus_step.h>

#define DELAY_BEFORE_SM 30
#define INSTRUCTION_NUMBER_JMP 36

int instruction_counter = 0;
uint16_t **dma_addr      = (uint16_t**) 0x0070;
uint16_t *dma_countdown = (uint16_t*) 0x0072;
uint16_t *dma_trace     = (uint16_t*) 0x0074;
uint16_t delay = 0;

DECLARE_SM(foo, 0x1234);

void SM_ENTRY(foo) test(char key) {
    char * p = &key;
    __asm__ __volatile__(
        "mov %0, r6\n\t"
        "mov #0x42, r7\n\t"
        "cmp.b @r6+, r7\n\t"
        "jz 1f\n\t"
        "bis #0x1, r7\n\t"
        "jmp 2f\n\t"
        "1: nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "2: nop\n\t"
        :
        :"m"(p)
        :"r6", "r7"
    );
}

/*
 * For the JMP 2f instruction, the program memory trace will be 00001111 11000000
 * For the NOP    instruction, the program memory trace will be 00001111 10000000
 *
 * By checking the different bit at the 0x40 position, we can tell which branch
 * was taken (the JPM is executed if the guess was incorrect, after the
 * bit is set).
 */

void irqHandler(void)
{
    delay = __ss_isr_reti_latency + DELAY_BEFORE_SM;
    uint16_t *pmem_addr = (uint16_t*) irqHandler;
    if (instruction_counter == INSTRUCTION_NUMBER_JMP) {
        if (*dma_trace & 0x40) {
            pr_info("Key was not guessed!");
        } else {
            pr_info("Key was guessed!");
        }
    }
    ++instruction_counter;
    *dma_addr = pmem_addr;
    *dma_countdown = delay;
}

int main()
{
    msp430_io_init();
    asm("eint\n\t");
    sancus_enable(&foo);

    __ss_start();
    test(0x41);
    instruction_counter = 0;
    __ss_start();
    test(0x42);

    EXIT();
}

/* ======== TIMER A ISR ======== */
SANCUS_STEP_ISR_ENTRY2(irqHandler, __ss_end)
