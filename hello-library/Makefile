include ../Makefile.include # Examples include
include ./Makefile.include  # Library project include

SOURCES         = $(shell ls *.c)
OBJECTS         = $(SOURCES:.c=.o)

TARGET          = main.elf
TARGET_NO_WRAP  = no_wrap_$(TARGET)

# We need to give the full path of the library so that the linker can detect
LDFLAGS += -L$(LIBRARY_FOLDER) -l:$(CURDIR)/$(LIBRARY_FOLDER)/$(LIBRARY_FILENAME) --scan-libraries-for-sm 
CFLAGS  += -I$(LIBRARY_FOLDER) 

.PHONY: all $(LIBRARY_FOLDER) thin sim sim-thin

all: normal-archive $(TARGET) 
thin: thin-archive $(TARGET)
sim: normal-archive simulate-normal
sim-thin: thin-archive simulate-thin

normal-archive: $(LIBRARY_FOLDER)
	$(MAKE) -C $(LIBRARY_FOLDER) all

thin-archive: $(LIBRARY_FOLDER)
	$(MAKE) -C $(LIBRARY_FOLDER) thin

$(TARGET_NO_WRAP): $(OBJECTS)
	$(LD) $(LDFLAGS) --prepare-for-sm-text-section-wrapping -o $@ $^

$(TARGET): $(TARGET_NO_WRAP)
	$(SANCUS_CRYPTO) --wrap-sm-text-sections $(CRYPTOFLAGS) -o $@ $<

load: $(TARGET)
	$(SANCUS_LOAD) $(LOADFLAGS) $<

simulate-normal: $(TARGET)
	$(SANCUS_SIM) $(SIMFLAGS) $<

simulate-thin: $(TARGET)
	$(SANCUS_SIM) $(SIMFLAGS) $<

clean:
	$(RM) $(TARGET) $(TARGET_NO_WRAP) $(OBJECTS)
	$(MAKE) -C $(LIBRARY_FOLDER) clean
	rm -f sim-input.bin sim-output.bin
	rm -f *.fst *.vcd
