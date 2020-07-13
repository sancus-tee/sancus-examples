include ../Makefile.include

SIMFLAGS       += --fileio-in sim-input.bin --fileio-out sim-output.bin

all: ping.elf pong.elf

no_mac_%.elf: %.o
	$(LD) $(LDFLAGS) -ldev-fileio -o $@ $^

%.elf: no_mac_%.elf
	$(SANCUS_CRYPTO) --fill-macs $(CRYPTOFLAGS) -o $@ $<

load: $(TARGET)
	@echo "error: fileio peripheral only supported in simulator.."

sim: ping.elf pong.elf
	@rm -f sim-input.bin sim-output.bin
	@touch sim-input.bin sim-output.bin
	$(SANCUS_SIM) $(SIMFLAGS) --fileio-in sim-output.bin --fileio-out sim-input.bin pong.elf &
	$(SANCUS_SIM) $(SIMFLAGS) --fileio-in sim-input.bin --fileio-out sim-output.bin ping.elf
	@echo -n "ping output is: '" ; cat sim-output.bin ; echo "'"
	@echo -n "pong output is: '" ; cat sim-input.bin ; echo "'"

clean:
	$(RM) *.elf
	rm -f sim-input.bin sim-output.bin
	rm -f *.fst *.vcd
