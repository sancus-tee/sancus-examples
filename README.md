# sancus-examples
[![Build Status](https://travis-ci.org/sancus-pma/sancus-examples.svg?branch=master)](https://travis-ci.org/sancus-pma/sancus-examples)

Elementary example programs to test your Sancus distribution.

## Overview

- **hello-world:** minimal working example of a Sancus program with an SM that
  dumps "hello world" on (untrusted) stdout.
- **sensor-reader:** example program from the paper, demonstrating secure linking
  between a reader SM and a secure memory-mapped I/O sensor SM.
- **arithmetic:** dummy SMs performing secure (inlined) arithmetic operations.
- **sancus-contiki:** Supporting files for some of the demos: port of the
  Contiki OS with support for Sancus modules.
    - **examples/hello-world:** minimal example of an SM
      that can be invoked from a Contiki task.
    - **examples/sm-server:** Sancus module loader and minimalist
      infrastructure for reactive SM programming.
    - **ta-demo:** "Lightweight and flexible trust assessment modules
      for the Internet of Things", [ESORICS 2015](https://distrinet.cs.kuleuven.be/software/sancus/publications/esorics15.pdf).

## Running the Examples

All example programs can be built for either the Sancus Verilog simulator, or
for an FPGA. To run the example in `sancus-sim`, proceed as follows:

```bash
$ make clean sim
```

Alternatively, to upload the example to an FPGA using `sancus-loader`:

```bash
$ make clean load
```
