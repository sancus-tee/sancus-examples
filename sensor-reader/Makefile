include ../Makefile.include

SOURCES         = $(shell ls *.c)
OBJECTS         = $(SOURCES:.c=.o)

TARGET          = main.elf
TARGET_NO_MAC   = no_mac_$(TARGET)

READER_SM_KEY   = `$(SANCUS_CRYPTO) --gen-sm-key reader --key $(VENDOR_KEY) main.elf | xxd -p`

all: $(TARGET)

$(TARGET_NO_MAC): $(OBJECTS)
	$(LD) $^ $(LDFLAGS) -o $@

$(TARGET): $(TARGET_NO_MAC)
	$(SANCUS_CRYPTO) --fill-macs $(CRYPTOFLAGS) -o $@ $<

load: $(TARGET)
	$(SANCUS_LOAD) $(LOADFLAGS) $<

# redirect output to file and stdout (apt-get install expect)
# https://stackoverflow.com/a/7219252
sim: $(TARGET)
	unbuffer $(SANCUS_SIM) $(SIMFLAGS) $< | tee sim.out
	./unwrap.py $(SANCUS_CRYPTO) $(READER_SM_KEY) sim.out

clean:
	$(RM) $(TARGET) $(TARGET_NO_MAC) $(OBJECTS)
	$(RM) sim-input.bin sim-output.bin sim.out
	$(RM) *.fst *.vcd
	$(RM) *.ll
	$(RM) *.s
