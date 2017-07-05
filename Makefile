
EXAMPLES = sensor-reader hello-world


all: ${EXAMPLES}


.PHONY: ${EXAMPLES}

sensor-reader:
	${MAKE} -C $@ ${MODE}

hello-world:
	${MAKE} -C $@ ${MODE}



clean:
	for dir in ${EXAMPLES}; do \
          ${MAKE} -C "$$dir" clean; \
        done

distclean: clean


