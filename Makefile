SUPPORT  = sancus-contiki
EXAMPLES = hello-world sensor-reader
EXAMPLES_CONTIKI \
         = contiki-hello-world contiki-sm-server ta-demo
CONTIKI_EX_PATH = sancus-contiki/examples

all: ${SUPPORT} ${EXAMPLES} ${EXAMPLES_CONTIKI}

.PHONY: ${SUPPORT} ${EXAMPLES} ${EXAMPLES_CONTIKI}

sancus-contiki:
	git submodule init
	git submodule update
	${MAKE} -C $@ update

sensor-reader:
	${MAKE} -C $@ ${MODE}

hello-world:
	${MAKE} -C $@ ${MODE}

contiki-hello-world: sancus-contiki
	${MAKE} TARGET=sancus-fpga -C ${CONTIKI_EX_PATH}/hello-world

contiki-sm-server: sancus-contiki
	${MAKE} TARGET=sancus-fpga -C ${CONTIKI_EX_PATH}/sm-server

ta-demo: sancus-contiki
	${MAKE} -C $@
#	${MAKE} TARGET=sancus-fpga -C ${CONTIKI_EX_PATH}/ta_mod clean
#	${MAKE} TARGET=sancus-fpga -C ${CONTIKI_EX_PATH}/ta_mod

clean: sancus-contiki-clean
	for dir in ${EXAMPLES}; do \
          ${MAKE} -C "$$dir" clean; \
        done

sancus-contiki-clean: sancus-contiki
	${MAKE} TARGET=sancus-fpga -C ${CONTIKI_EX_PATH}/hello-world clean
	${MAKE} TARGET=sancus-fpga -C ${CONTIKI_EX_PATH}/sm-server clean

distclean: clean
