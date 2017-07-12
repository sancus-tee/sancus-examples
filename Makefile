SUPPORT  = sancus-contiki
EXAMPLES = hello-world sensor-reader
EXAMPLES_CONTIKI \
         = contiki-hello-world contiki-sm-server ta-demo

all: ${SUPPORT} ${EXAMPLES}

.PHONY: ${SUPPORT} ${EXAMPLES}

sancus-contiki:
	git submodule init
	git submodule update
	${MAKE} -C $@ update

sensor-reader:
	${MAKE} -C $@ ${MODE}

hello-world:
	${MAKE} -C $@ ${MODE}

ta-demo: sancus-contiki
	${MAKE} -C $@ ${MODE}


clean:
	for dir in ${EXAMPLES}; do \
          ${MAKE} -C "$$dir" clean; \
        done

distclean: clean
