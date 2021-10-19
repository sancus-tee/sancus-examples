include ../Makefile.include

SOURCES         = $(shell ls *.c)
OBJECTS         = $(SOURCES:.c=.o)

TARGET          = main.elf
TARGET_NO_MAC   = no_mac_$(TARGET)

LDFLAGS += --sm-config-file=./sm-config.yml --debug

all: $(TARGET)

$(TARGET_NO_MAC): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(TARGET): $(TARGET_NO_MAC)
	$(SANCUS_CRYPTO) --fill-macs $(CRYPTOFLAGS) -o $@ $<

load: $(TARGET)
	$(SANCUS_LOAD) $(LOADFLAGS) $<

# redirect output to file and stdout (apt-get install expect)
# https://stackoverflow.com/a/7219252
sim: $(TARGET)
	unbuffer $(SANCUS_SIM) $(SIMFLAGS) --stop-after-sm-violation=-1 $< | tee sim.out

clean:
	$(RM) $(TARGET) $(TARGET_NO_MAC) $(OBJECTS)
	rm -f sim-input.bin sim-output.bin sim.out
	rm -f *.fst *.vcd
