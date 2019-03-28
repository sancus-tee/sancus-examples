sllvm_pm_foo_data_start = .;
*(.sllvm.data.foo)
*(.sllvm.data.lr1addr.foo) /* Make sure the pointer to local sp is put last */
sllvm_pm_foo_data_end = .;

sllvm_pm_sancus_step_data_start = .;
*(.sllvm.data.sancus_step)
*(.sllvm.data.lr1addr.sancus_step) /* Make sure pointer to local sp is last */
sllvm_pm_sancus_step_data_end = .;
