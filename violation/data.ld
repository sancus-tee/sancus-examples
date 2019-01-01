sllvm_pm_foo_data_start = .;
*(.sllvm.data.foo)
*(.sllvm.data.lr1addr.foo) /* Make sure the pointer to local sp is put last */
sllvm_pm_foo_data_end = .;
