sllvm_pm_foo_text_start = .;
*(.sllvm.text.dispatch.foo)  /* Make sure the dispatcher is put first */
*(.sllvm.text.foo)
. += 2;                      /* Prevents this region from begin empty. The 
                                linker garbage collector removes the foo text
                                sections because they are never used. */
sllvm_pm_foo_text_end = .;
