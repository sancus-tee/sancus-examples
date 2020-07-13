include ../Makefile.include # Examples include
include ./Makefile.include  # Library project include

ifdef SLLVM_INSTALL_DIR
sim: 
clean:
else
  LDFLAGS += --prepare-for-sm-text-section-wrapping
  LDFLAGS += --scan-libraries-for-sm 

SOURCES         = $(shell ls *.c)
OBJECTS         = $(SOURCES:.c=.o)

TARGET          = main.elf
TARGET_NO_WRAP  = no_wrap_$(TARGET)

# This specifies how the library is built. A non defined ARCHIVE_MODE specifies normal mode, anything else specifies thin mode
ifeq ($(ARCHIVE_MODE), )
	ARCHIVE_MODE = thick
	LIB_MODE = all
	MODE_NAME = "normal, thick archive"
else
	ARCHIVE_MODE = thin
	LIB_MODE = thin
	MODE_NAME = "thin archive"
endif

# We need to give the full path of the library so that the linker can detect
LDFLAGS += -L$(LIBRARY_FOLDER) -l:$(CURDIR)/$(LIBRARY_FOLDER)/$(LIBRARY_FILENAME)
CFLAGS  += -I$(LIBRARY_FOLDER) 

.PHONY: all $(LIBRARY_FOLDER) sim 

all: archive $(TARGET) 
sim: archive run-simulation

archive: $(LIBRARY_FOLDER)
	@echo "Building library as a $(MODE_NAME)"
	$(MAKE) -C $(LIBRARY_FOLDER) $(LIB_MODE)

$(TARGET_NO_WRAP): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(TARGET): $(TARGET_NO_WRAP)
	$(SANCUS_CRYPTO) --wrap-sm-text-sections $(CRYPTOFLAGS) -o $@ $<

load: thin-archive $(TARGET)
	$(SANCUS_LOAD) $(LOADFLAGS) $<

run-simulation: $(TARGET)
	$(SANCUS_SIM) $(SIMFLAGS) $<

clean:
	$(RM) $(TARGET) $(TARGET_NO_WRAP) $(OBJECTS)
	$(MAKE) -C $(LIBRARY_FOLDER) clean
	rm -f sim-input.bin sim-output.bin
	rm -f *.fst *.vcd
endif
