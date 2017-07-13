include ../Makefile.include

SOURCES         = $(shell ls *.c) ../common.c
OBJECTS         = $(SOURCES:.c=.o)

TARGET          = main.elf
TARGET_NO_MAC   = no_mac_$(TARGET)

# data below as printed when executing the program
NONCE           = cdab
CIPHER          = 3a36
TAG             = cd027bfe3a117f17
READER_SM_KEY   = `$(SANCUS_CRYPTO) --gen-sm-key reader --key $(VENDOR_KEY) main.elf | xxd -p`

all: $(TARGET)

$(TARGET_NO_MAC): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(TARGET): $(TARGET_NO_MAC)
	$(SANCUS_CRYPTO) $(CRYPTOFLAGS) -o $@ $<

load: $(TARGET)
	$(SANCUS_LOAD) $(LOADFLAGS) $<

sim: $(TARGET)
	$(SANCUS_SIM) $(SIMFLAGS) $<

verify: $(TARGET)
	sancus-crypto --unwrap $(NONCE) $(CIPHER) $(TAG) --key $(READER_SM_KEY)

ifeq ($(SIM),1)
run: sim
else
run: load
endif

clean:
	$(RM) $(TARGET) $(TARGET_NO_MAC) $(OBJECTS)
	rm -f sim-input.bin sim-output.bin
	rm -f *.fst *.vcd
