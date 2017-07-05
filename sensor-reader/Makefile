
include ../Makefile.include


OBJECTS=main.o sensor.o reader.o
TARGET=main.elf
TARGET_NO_MACS=$(TARGET).no-macs
VKEY=4078d505d82099ba
#VKEY=d049567a671b0a811cce5056
#VKEY=881b0f0a3166f65275db5952fee66ac7
#VKEY=e66bd0f166138667103eefc1c9d07681df48b44339849ff824885b1227034941
ROM=48K
RAM=10K

CFLAGS=
LIBS=

all: $(TARGET)

$(TARGET_NO_MACS): $(OBJECTS) $(LIBS)
	$(LD) --standalone --rom $(ROM) --ram $(RAM) -o $@ $^

$(TARGET): $(TARGET_NO_MACS)
	$(SANCUSCRYPTO) --fill-macs --key $(VKEY) -o $@ $<

sim: $(TARGET)
	$(SANCUSSIM) --rom $(ROM) --ram $(RAM) $<

load: $(TARGET)
	$(SANCUSLOAD) -device ${FPGATTY} -baudrate ${FPGARATE} $<

clean:
	rm -f $(OBJECTS) $(TARGET_NO_MACS) $(TARGET)
	rm -f sim-input.bin sim-output.bin
	rm -f *.fst *.vcd

