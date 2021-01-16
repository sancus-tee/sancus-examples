include ../Makefile.include

SOURCES         = $(shell ls *.c)
OBJECTS         = $(SOURCES:.c=.o)

TARGET          = main.elf
TARGET_NO_WRAP  = no_wrap_$(TARGET)

all: $(TARGET)

$(TARGET_NO_WRAP): $(OBJECTS)
	$(LD) $(LDFLAGS) --prepare-for-sm-text-section-wrapping -o $@ $^

$(TARGET): $(TARGET_NO_WRAP)
	$(SANCUS_CRYPTO) --wrap-sm-text-sections $(CRYPTOFLAGS) -o $@ $<

load: $(TARGET)
	$(SANCUS_LOAD) $(LOADFLAGS) $<

sim: $(TARGET)
	$(SANCUS_SIM) $(SIMFLAGS) $<

clean:
	$(RM) $(TARGET) $(TARGET_NO_MAC) $(OBJECTS)
	rm -f sim-input.bin sim-output.bin
	rm -f *.fst *.vcd
