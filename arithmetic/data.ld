sllvm_pm_bar_data_start = .;
*(.sllvm.data.bar)
*(.sllvm.data.lr1addr.bar) /* Make sure the pointer to local sp is put last */
sllvm_pm_bar_data_end = .;

sllvm_pm_foo_data_start = .;
*(.sllvm.data.foo)
*(.sllvm.data.lr1addr.foo) /* Make sure the pointer to local sp is put last */
sllvm_pm_foo_data_end = .;
