
all: install-pkg sim.ta_demo.openmsp430


HTTPPATH = https://people.cs.kuleuven.be/~job.noorman

download-pkg:
	wget ${HTTPPATH}/sancus-support.deb
	wget ${HTTPPATH}/sancus-core.deb
	wget ${HTTPPATH}/sancus-compiler.deb
	wget ${HTTPPATH}/llvm-sancus.deb
	touch download-pkg

install-pkg: download-pkg
	sudo dpkg -i sancus-support.deb sancus-core.deb \
          sancus-compiler.deb llvm-sancus.deb; true \
	sudo apt-get install -f	
	sudo pip3 install pyelftools
	touch install-pkg


clean: ta_demo.clean
	rm -f *.deb
	rm -f sancus_sim.fst sim-input.bin sim-output.bin

distclean: clean
	rm -f download-pkg install-pkg


SET_ENV      = 
#SET_ENV      = $(shell cat ../sancus-setenv)
SANCUSMAKE   = ${SET_ENV} ${MAKE} SANCUS=1

SANCUS_DIR       = /usr/
MSP_SUPPORT_DIR  = $(SANCUS_DIR)/share/sancus-support
TOOLS_DIR        = $(MSP_SUPPORT_DIR)/tools
INCLUDES_DIR     = $(MSP_SUPPORT_DIR)/include
LIBS_DIR         = $(MSP_SUPPORT_DIR)/lib


ifeq ($(SANCUS), 1)
CC           = sancus-cc
LD           = sancus-ld
endif

GEN_SYMTAB   = $(TOOLS_DIR)/generate-symtab.py
SIM          = sancus-sim
CRYPTO       = sancus-crypto
LOAD         = sancus-loader

DEFAULT_ROM  = 48K
DEFAULT_RAM  = 10K
DEV          = /dev/ttyUSB0
BAUD         = 115200


include Makefile.ta_mod


