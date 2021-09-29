include ../Makefile.include

SOURCES         = $(shell find *.c ! -name "sm1.c" -printf "%p ")
OBJECTS         = $(SOURCES:.c=.o)

TARGET          = main.elf
TARGET_NO_MAC   = no_mac_$(TARGET)

LDFLAGS        += -lsm-control -lloader -ltools

all: $(TARGET)

sm1.elf: sm1.o
	$(LD) --verbose -o $@ $<
	# print undefined symbols
	nm -u $<

sm1.h: hardcode_elf.py sm1.elf 
	python3 $<
	# dump generated header file
	cat $@

$(TARGET_NO_MAC): sm1.h $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(TARGET): $(TARGET_NO_MAC)
	$(SANCUS_CRYPTO) --fill-macs $(CRYPTOFLAGS) -o $@ $<

load: $(TARGET)
	$(SANCUS_LOAD) $(LOADFLAGS) $<

sim: $(TARGET)
	$(SANCUS_SIM) $(SIMFLAGS) $<

clean:
	$(RM) $(TARGET) $(TARGET_NO_WRAP) $(OBJECTS)
	$(RM) sim-input.bin sim-output.bin
	$(RM) *.fst *.vcd
	$(RM) sm1.h sm1.o sm1.elf
