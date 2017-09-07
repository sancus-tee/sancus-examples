include ../Makefile.include

SOURCES         = $(shell ls *.c)
OBJECTS         = $(SOURCES:.c=.o)

TARGET          = main.elf
TARGET_NO_MAC   = no_mac_$(TARGET)

TEST_DATA       = "hello"
CFLAGS         += -DTEST_DATA=\"$(TEST_DATA)\"

all: $(TARGET)

$(TARGET_NO_MAC): $(OBJECTS)
	$(LD) $(LDFLAGS) -ldev-fileio -o $@ $^

$(TARGET): $(TARGET_NO_MAC)
	$(SANCUS_CRYPTO) --fill-macs $(CRYPTOFLAGS) -o $@ $<

load: $(TARGET)
	@echo "error: fileio peripheral only supported in simulator.."

sim: $(TARGET)
	@rm -f sim-input.bin sim-output.bin
	@echo -n $(TEST_DATA) > sim-input.bin
	$(SANCUS_SIM) $(SIMFLAGS) $<
	@echo -n "sim output is: '" ; cat sim-output.bin ; echo "'"

clean:
	$(RM) $(TARGET) $(TARGET_NO_MAC) $(OBJECTS)
	rm -f sim-input.bin sim-output.bin
	rm -f *.fst *.vcd
