
include ../Makefile.include


CONTIKIDIR = ../sancus-contiki/
TAMOD      = examples/ta_mod


all: build

build: sim.ta_demo.openmsp430

sim:

load:

clean:

distclean:



.PHONY: copy-src
copy-src_ta_mod:
	cp -r ta_mod ${CONTIKIDIR}/examples/

ta_demo.openmsp430: copy-src_ta_mod
	${MAKE} V=1 -C ${CONTIKIDIR}/${TAMOD} \
          TARGET=sancus-fpga SANCUS_DIR="$$SANCUS_DIR"
	cp ${CONTIKIDIR}/${TAMOD}/ta_demo.openmsp430 ./
	msp430-size --format=SysV -t -d ta_demo.openmsp430

ta_demo.openmsp430.elf: ta_demo.openmsp430 vendor-key.txt
	${SANCUSCRYPTO} --key $(shell cat ./vendor-key.txt) \
          --fill-macs ta_demo.openmsp430 -o ta_demo.openmsp430.elf

.PHONY: sim.ta_demo.openmsp430
sim.ta_demo.openmsp430:
	${MAKE} clean
	${MAKE} ta_demo.openmsp430.elf BUILD=sim
	touch sim.ta_demo.openmsp430
	$(SANCUSSIM) --rom 41K --ram 16K ta_demo.openmsp430.elf; \
          true
	rm -f sim.ta_demo.openmsp430

.PHONY: load.ta_demo.openmsp430
load.ta_demo.openmsp430:
	${MAKE} clean
	${MAKE} ta_demo.openmsp430.elf BUILD=load
	${SANCUSLOAD} -device $(FPGATTY) -baudrate $(FPGARATE) \
          --rom 41K --ram 16K ta_demo.openmsp430.elf
	screen -L $(FPGATTY) $(FPGARATE)

ta_mod.keys: vendor-key.txt
	@echo "+++ ta_mod"
	${SANCUSCRYPTO} --key $(shell cat ./vendor-key.txt) \
          --gen-sm-key TACore \
          ta_demo.openmsp430.elf
	@echo "Now copy and paste this to 'ta_demo-key.txt'!"

# sancus-crypto --key a39ebb7f80ce2b03 --unwrap cipher tag
# vendor-key.txt:
# sancus-crypto --key deadbeefcafebabe --gen-vendor-key 1234

ta_demo.clean:
	rm -f ta_demo.openmsp430 ta_demo.openmsp430.elf
	if [ -d ${CONTIKIDIR}/${TAMOD} ]; then \
	  ${MAKE} -C ${CONTIKIDIR}/${TAMOD} clean \
            TARGET=sancus-fpga BUILD=load SANCUS_DIR=${SANCUS_DIR}; \
	fi
	rm -rf ${CONTIKIDIR}/${TAMOD}
	rm -f sim.ta_demo.openmsp430 load.ta_demo.openmsp430
	rm -f ta_demo/util/*.o

ta_demo.distclean: ta_demo.clean


