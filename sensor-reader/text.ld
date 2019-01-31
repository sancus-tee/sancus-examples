sllvm_pm_reader_text_start = .;
*(.sllvm.text.dispatch.reader)  /* Make sure the dispatcher is put first */
*(.sllvm.text.reader)
sllvm_pm_reader_text_end = .;

sllvm_pm_sensor_text_start = .;
*(.sllvm.text.dispatch.sensor)  /* Make sure the dispatcher is put first */
*(.sllvm.text.sensor)
sllvm_pm_sensor_text_end = .;

/* Symbols for secure linking */
*(.sllvm.slink.*)
