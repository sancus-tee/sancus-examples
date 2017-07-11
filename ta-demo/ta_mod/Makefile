
include Makefile.include



WITH_ANTELOPE         = 0
WITH_EXAMPLE_NET      = 0
CONTIKI_NO_NET        = 1


PROCESS_INSPECTOR     = 1
SCHEDULER_PROTECTION  = 1
PROTECT_IVTBL         = 1



ifneq ($(TARGET),sancus-fpga)
  $(error This example is designed for the OpenMSP430 with Sancus extensions only. Other targets are not supported)
endif


ifdef SANCUS_DIR
  MSP_SUPPORT_DIR  = $(SANCUS_DIR)/share/sancus-support
  TOOLS_DIR        = $(MSP_SUPPORT_DIR)/tools
  SUPPORT_INC_DIR  = $(MSP_SUPPORT_DIR)/include
  LIBS_DIR         = $(MSP_SUPPORT_DIR)/lib

  SANCUS_INC_DIR   = $(SANCUS_DIR)/share/sancus-compiler/include
  CFLAGS          += -I${SUPPORT_INC_DIR} -I$(SANCUS_INC_DIR)

  GEN_SYMTAB       = $(TOOLS_DIR)/generate-symtab.py
else
  $(error SANCUS_DIR not defined!)
endif


ifndef BUILD
  $(error BUILD not defined!)
endif
ifeq ($(BUILD),sim)
MSP430_ROM       = 41K
MSP430_RAM       = 16K
CFLAGS          += -D__SANCUS_SIM
WITH_SLIP        = 0
endif
ifeq ($(BUILD),load)
  # Build with network?
  ifeq ($(CONTIKI_NO_NET),0)
    WITH_SLIP        = 1
  else
    MSP430_ROM       = 41K
    MSP430_RAM       = 16K
    WITH_SLIP        = 0
  endif
CFLAGS          += -D__SANCUS_LOAD
LIBS            += $(LIBS_DIR)/libdev-uart.a
endif

LIBS            += $(LIBS_DIR)/libsm-control.a $(LIBS_DIR)/libloader.a \
		   $(LIBS_DIR)/libmalloc.a

# Should point to Contiki's <GIT_ROOT>/contiki
CONTIKI = ../../contiki
# Should point to <GIT_ROOT>/targets
TARGETDIRS += ../../targets
# Should point to <GIT_ROOT>/apps
APPDIRS += ../../apps



ifeq ($(WITH_SLIP),1)
  CONTIKI_NO_NET = 0
  WITH_UIP       = 1
  UIP_CONF_IPV6  = 0
  UIP_CONF_IPV4  = 1
  CONTIKI_WITH_IPV4=1
  CFLAGS += -DCONTIKI_WITH_IPV4=1 -DWITH_UIP=1 -DWITH_SLIP=1
  ifeq ($(WITH_EXAMPLE_NET), 1)
    CFLAGS += -DWITH_EXAMPLE_NET=1
    PROJECT_SOURCEFILES += apps/networking.c
  endif
endif

ifeq (${WITH_ANTELOPE}, 1)
  CFLAGS += -DWITH_ANTELOPE=1
  APPS   += antelope
  PROJECT_SOURCEFILES += apps/shell-db.c
endif


SYMTAB               = sancus-symtab.c

PROJECT_SOURCEFILES += ta_mod.c ta_mod_process.c ta_helpers.c \
                       ta_proc.c ta_invar.c \
                       apps/hello-world.c apps/multi-threading.c \
                       apps/attacker.c

CONTIKI_PROJECT      = ta_demo

TARGET_LIBFILES     += ${LIBS}

SANCUS_MODULES       = 

ifeq (${PROCESS_INSPECTOR}, 1)
CFLAGS              += -DTA_PROCESS_INSPECTOR=1
endif

ifeq (${SCHEDULER_PROTECTION}, 1)
CFLAGS              += -DTA_SCHEDULER_PROTECTION=1
CFLAGS              += -DBUILTIN_PROT_SCHED=1
endif

ifeq (${PROTECT_IVTBL}, 1)
CFLAGS              += -DPROTECT_IVTBL=1
endif


# --- what to make -----------------------------------------------------------

all: setup $(CONTIKI_PROJECT) $(SANCUS_MODULES)

include $(CONTIKI)/Makefile.include


# --- custom rules -----------------------------------------------------------

setup:
	mkdir -p $(OBJECTDIR)/apps

# Essentially, compillation units that are to be compiled with sancus-cc
# should be named ta_* -- sancus-cc doesn't work for compiling all of
# Contiki.
$(OBJECTDIR)/ta_%.o: ta_%.c | $(OBJECTDIR)
	echo ******************* ${SANCUSCC} 
	${SANCUSCC} --debug $(CFLAGS) -MMD -c $< -o $@


# --- modules and similar targets --------------------------------------------

$(SYMTAB): sancus-symtab.txt
	$(GEN_SYMTAB) -o $@ \
          sancus-symtab.txt

ta-upload: ta-upload.c
	gcc ta-upload.c -o ta-upload

