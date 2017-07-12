
ifneq ($(TARGET),sancus-fpga)
  $(error This example is designed for the OpenMSP430 with Sancus extensions only. Other targets are not supported)
endif


ifeq ($(TARGET),sancus-fpga)
  CFLAGS += -D__SANCUS_LOAD=1
endif


# Should be the name of the main .c file without the extension
CONTIKI_PROJECT = ta-demo
all: $(CONTIKI_PROJECT)

APPS = 

# Should point to <GIT_ROOT>/targets
TARGETDIRS += ../../targets
# Should point to <GIT_ROOT>/apps
APPDIRS += ../../apps
# Should point to <GIT_ROOT>/contiki
CONTIKI = ../../contiki

PROJECT_SOURCEFILES = attacker.c multi-threading.c hello-world.c

ifndef NO_SANCUS
SM_FILES = ta_demo.c ta_mod.c ta_mod_process.c ta_helpers.c \
  ta_proc.c ta_invar.c
else
SM_FILES =
PROJECT_SOURCEFILES +=
endif

CONTIKI_WITH_IPV4 = 1
include $(CONTIKI)/Makefile.include


all: setup


