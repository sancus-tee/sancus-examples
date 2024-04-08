.global __sm_foo_entry
.global enter_foo
.global foo_c_fct
.global foo_exit_trampoline

/* PROTECTED ENCLAVE CODE */
.section ".sm.foo.text", "wax"
.align 2
__sm_foo_entry:
enter_foo:
    nop
    cmp #0, r15
    jz 1f
    mov #0xbe00, r15
    mov #0x00ef, r14

    /*
     * NOTE: this is deliberately _insecure_ as the stack pointer is not
     * switched to a private stack(!)
     */
    call #foo_c_fct

    jmp 2f
1:
    mov #0xdead, r15
2:
    br #foo_exit_trampoline

/* UNPROTECTED CODE OUTSIDE ENCLAVE */
.text
foo_exit_trampoline:
    ; NOTE: we do the ret _outside_ of the enclave, as the stack pointer is
    ; untrusted and could be poisoned to point _inside_ the enclave..
    ret

