
include ../Makefile.include


OBJECTS=main.o hello.o
TARGET=main.elf
TARGET_NO_MACS=$(TARGET).no-macs
ROM=48K
RAM=10K

CFLAGS=
LIBS=

all: $(TARGET)

build: all

$(TARGET_NO_MACS): $(OBJECTS) $(LIBS)
	$(LD) --standalone --rom $(ROM) --ram $(RAM) -o $@ $^

$(TARGET): $(TARGET_NO_MACS)
	cp $< $@

sim: $(TARGET)
	$(SANCUSSIM) --rom $(ROM) --ram $(RAM) $<

load: $(TARGET)
	$(SANCUSLOAD) -device ${FPGATTY} -baudrate ${FPGARATE} $<

clean:
	rm -f $(OBJECTS) $(TARGET_NO_MACS) $(TARGET)
	rm -f sim-input.bin sim-output.bin
	rm -f *.fst *.vcd

