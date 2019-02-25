include ../Makefile.include

SOURCES         = $(shell ls *.c)
OBJECTS         = $(SOURCES:.c=.o)

TARGET          = main.elf
TARGET_NO_WRAP  = no_wrap_$(TARGET)

all: $(TARGET)

$(TARGET_NO_WRAP): $(OBJECTS)
	$(LD) $(LDFLAGS) -lsancus-step -o $@ $^ 

$(TARGET): $(TARGET_NO_WRAP)
	$(SANCUS_CRYPTO) $(CRYPTOFLAGS) --fill-macs -o $@ $<

load: $(TARGET)
	$(SANCUS_LOAD) $(LOADFLAGS) $<

sim: $(TARGET)
	$(SANCUS_SIM) $(SIMFLAGS) $<

clean:
	$(RM) $(TARGET) $(TARGET_NO_WRAP) $(OBJECTS)
	rm -f sim-input.bin sim-output.bin
	rm -f *.fst *.vcd
