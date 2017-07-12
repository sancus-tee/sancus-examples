# sancus-examples

Elementary example programs to test your Sancus distribution.

## Overview

- **hello-world:** minimal working example of a Sancus program with an SM that
  dumps "hello world" on (untrusted) stdout.
- **sensor-reader:** example program from the paper, demonstrating secure
  linking between dummy sensor/reader SMs.

## Running the Examples

All example programs can be built for either the Sancus Verilog simulator, or
for an FPGA. To run the example in `sancus-sim`, proceed as follows:

```bash
$ make SIM=1 clean sim
```

Alternatively, to upload the example to an FPGA using `sancus-loader`:

```bash
$make clean load
```
