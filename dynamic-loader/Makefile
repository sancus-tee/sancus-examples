include ../Makefile.include

SOURCES         = $(shell find *.c ! -name "sm1.c" -printf "%p ")
OBJECTS         = $(SOURCES:.c=.o)
SM1_HEADER      = $(shell python3 hardcode_elf.py)

TARGET          = main.elf
TARGET_NO_WRAP  = no_wrap_$(TARGET)

LDFLAGS += -lsm-control -lloader -ltools

all: $(TARGET)

$(TARGET_NO_WRAP): $(SM1_HEADER) $(OBJECTS)
	$(LD) $(LDFLAGS) --prepare-for-sm-text-section-wrapping -o $@ $^

$(TARGET): $(TARGET_NO_WRAP)
	$(SANCUS_CRYPTO) --wrap-sm-text-sections $(CRYPTOFLAGS) -o $@ $<

load: $(TARGET)
	$(SANCUS_LOAD) $(LOADFLAGS) $<

sim: $(TARGET)
	$(SANCUS_SIM) $(SIMFLAGS) $<

clean:
	$(RM) $(TARGET) $(TARGET_NO_WRAP) $(OBJECTS)
	rm -f sim-input.bin sim-output.bin
	rm -f *.fst *.vcd
	rm -f sm1.h sm1.o sm1.elf
