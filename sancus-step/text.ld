sllvm_pm_foo_text_start = .;
*(.sllvm.text.dispatch.foo)  /* Make sure the dispatcher is put first */
*(.sllvm.text.foo)
sllvm_pm_foo_text_end = .;

sllvm_pm_sancus_step_text_start = .;
*(.sllvm.text.dispatch.sancus_step)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sancus_step)
sllvm_pm_sancus_step_text_end = .;
