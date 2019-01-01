sllvm_pm_hello_text_start = .;
*(.sllvm.text.dispatch.hello)  /* Make sure the dispatcher is put first */
*(.sllvm.text.hello)
*(.sllvm.text.hello.*)         /* Internalized runtime functions */
sllvm_pm_hello_text_end = .;

/* Wrap information */
*(.sllvm.wrap.*)
