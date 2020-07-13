include ../Makefile.include

ifndef SLLVM_INSTALL_DIR
  LDFLAGS += --prepare-for-sm-text-section-wrapping
endif

SOURCES         = $(shell ls *.c)
OBJECTS         = $(SOURCES:.c=.o)

TARGET          = main.elf
TARGET_NO_WRAP  = no_wrap_$(TARGET)

all: $(TARGET)

$(TARGET_NO_WRAP): $(OBJECTS)
	$(LD) $^ $(LDFLAGS) -o $@

$(TARGET): $(TARGET_NO_WRAP)
	$(SANCUS_CRYPTO) --wrap-sm-text-sections $(CRYPTOFLAGS) -o $@ $<

load: $(TARGET)
	$(SANCUS_LOAD) $(LOADFLAGS) $<

sim: $(TARGET)
	$(SANCUS_SIM) $(SIMFLAGS) $<

clean:
	$(RM) $(TARGET) $(TARGET_NO_MAC) $(OBJECTS)
	$(RM) sim-input.bin sim-output.bin
	$(RM) *.fst *.vcd
	$(RM) *.ll
	$(RM) *.s
