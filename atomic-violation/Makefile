include ../Makefile.include

SOURCES         = $(shell ls *.c)
OBJECTS         = $(SOURCES:.c=.o)

TARGET          = main.elf
TARGET_NO_WRAP  = no_wrap_$(TARGET)

all: $(TARGET)

$(TARGET_NO_WRAP): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(TARGET): $(TARGET_NO_WRAP)
	$(SANCUS_CRYPTO) --fill-macs $(CRYPTOFLAGS) -o $@ $<

load: $(TARGET)
	$(SANCUS_LOAD) $(LOADFLAGS) $<

sim: $(TARGET)
	$(SANCUS_SIM) $(SIMFLAGS) --stop-after-sm-violation=-1 $<

clean:
	$(RM) $(TARGET) $(TARGET_NO_MAC) $(OBJECTS)
	rm -f sim-input.bin sim-output.bin
	rm -f *.fst *.vcd
