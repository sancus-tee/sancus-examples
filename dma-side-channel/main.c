#include <stdint.h>
#include <msp430.h>
#include <sancus/sm_support.h>
#include <sancus_support/dma_attacker.h>
#include <sancus_support/sm_io.h>
#include <sancus_support/sancus_step.h>

#define INSTRUCTION_NUMBER_JMP 36
#define TRACE_MASK 0x1000
int instruction_counter = 0;
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
 * For the JMP 2f instruction, the program memory trace will be 11110000 00000000
 * For the NOP    instruction, the program memory trace will be 11100000 00000000
 * The JMP is instruction is executed if the guess was incorrect, the NOP if it
 * was correct.
 *
 * Using TRACE_MASK to get the bit that differs in the two possible traces, we
 * can tell whether a collected trace was generated as a result of a correct or
 *  an incorrect guess.
 */

void irqHandler(void)
{
    __ss_set_dma_attacker_delay(0);
    uint16_t *pmem_addr = (uint16_t*) irqHandler;
    if (instruction_counter == INSTRUCTION_NUMBER_JMP) {
        if (DMA_TRACE & TRACE_MASK) {
            pr_info("Key was not guessed!");
        } else {
            pr_info("Key was guessed!");
        }
    }
    ++instruction_counter;
    DMA_ADDR = pmem_addr;
    DMA_COUNTDOWN = delay;
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
