
EXAMPLES = sensor-reader


all: ${EXAMPLES}


.PHONY: ${EXAMPLES}

sensor-reader:
	${MAKE} -C sensor-reader sim


clean:


