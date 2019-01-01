sllvm_pm_bar_text_start = .;
*(.sllvm.text.dispatch.bar)  /* Make sure the dispatcher is put first */
*(.sllvm.text.bar)
*(.sllvm.text.bar.*)         /* Internalized runtime functions */
sllvm_pm_bar_text_end = .;

sllvm_pm_foo_text_start = .;
*(.sllvm.text.dispatch.foo)  /* Make sure the dispatcher is put first */
*(.sllvm.text.foo)
*(.sllvm.text.foo.*)         /* Internalized runtime functions */
sllvm_pm_foo_text_end = .;

/* Hashes for secure linking */
*(.sllvm.hash.*)
