sllvm_pm_sensor_data_start = .;
*(.sllvm.data.sensor)
*(.sllvm.data.lr1addr.sensor) /* Make sure the pointer to local sp is last */
sllvm_pm_sensor_data_end = .;

sllvm_pm_reader_data_start = .;
*(.sllvm.data.reader)
*(.sllvm.data.lr1addr.reader) /* Make sure the pointer to local sp is last */
sllvm_pm_reader_data_end = .;
