include ../Makefile.include

SOURCES         = $(shell ls *.c) ../common.c
OBJECTS         = $(SOURCES:.c=.o)

TARGET          = main.elf
TARGET_NO_MAC   = no_mac_$(TARGET)
TARGET_WRAP     = wrap_$(TARGET)
TARGET_WRAPPED  = wrapped_$(TARGET)

all: $(TARGET)

$(TARGET_NO_MAC): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(TARGET): $(TARGET_NO_MAC)
	$(SANCUS_CRYPTO) $(CRYPTOFLAGS) -o $@ $<

$(TARGET_WRAP): $(OBJECTS)
	$(LD) $(LDFLAGS) --prepare-for-sm-text-section-wrapping -o $@ $^

$(TARGET_WRAPPED): $(TARGET_WRAP)
	$(SANCUS_CRYPTO) --wrap-sm-text-sections --key $(VENDOR_KEY) --verbose -o $@ $^

load: $(TARGET)
	$(SANCUS_LOAD) $(LOADFLAGS) $<

sim: $(TARGET)
	$(SANCUS_SIM) $(SIMFLAGS) $<

ifeq ($(SIM),1)
run: sim
else
run: load
endif

clean:
	$(RM) $(TARGET) $(TARGET_NO_MAC) $(TARGET_WRAP) $(TARGET_WRAPPED) $(OBJECTS)
	rm -f sim-input.bin sim-output.bin
	rm -f *.fst *.vcd
