include ../Makefile.include

SOURCES         = $(shell ls *.c)
STUB_SRC        = $(shell ls *.S)
OBJECTS         = $(SOURCES:.c=.o)
STUBS           = $(STUB_SRC:.S=.o)

TARGET          = main.elf
TARGET_NO_MAC   = no_mac_$(TARGET)

READER_SM_KEY   = `$(SANCUS_CRYPTO) --gen-sm-key reader --key $(VENDOR_KEY) main.elf | xxd -p`

all: $(TARGET)
	msp430-objdump -j .text.sm.foo -D main.elf

$(TARGET_NO_MAC): $(OBJECTS) $(STUBS)
	$(LD) $(LDFLAGS) --sm-config-file foo.yaml -o $@ $(OBJECTS)

$(TARGET): $(TARGET_NO_MAC)
	$(SANCUS_CRYPTO) --fill-macs $(CRYPTOFLAGS) -o $@ $<

load: $(TARGET)
	$(SANCUS_LOAD) $(LOADFLAGS) $<

sim: $(TARGET)
	$(SANCUS_SIM) $(SIMFLAGS) $<

clean:
	$(RM) $(TARGET) $(TARGET_NO_MAC) $(OBJECTS)
	rm -f sim-input.bin sim-output.bin sim.out
	rm -f *.fst *.vcd
