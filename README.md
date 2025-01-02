# sancus-examples
[![Run Sancus examples](https://github.com/sancus-tee/sancus-examples/actions/workflows/run-examples.yml/badge.svg)](https://github.com/sancus-tee/sancus-examples/actions/workflows/run-examples.yml)

Elementary example programs to test your Sancus distribution.

## Overview

The following programs showcase the security properties offered by a Sancus processor.

| Security feature          | Example program                | Comments                                                        |
|-------------------------- |--------------------------------|-----------------------------------------------------------------|
| Software module isolation | [hello-world](hello-world)     | Minimal working example to enable/disable a Sancus module.      |
| Secure linking            | [sensor-reader](sensor-reader) | Caller/callee authentication between sensor and reader SMs.     |
| Remote attestation        | [sensor-reader](sensor-reader) | Compute MAC over fresh challenge (nonce).                       |
| Secure communication      | [sensor-reader](sensor-reader) | Authenticated encryption with associated data primitive.        |
| Confidential deployment   | [confidential-loading](confidential-loading) | SM text section is transparently encrypted at build time.       |
| Secure interrupts         | [timer](timer)                 | Interruption and resumption of an application SM by untrusted ISR software. |

Building upon the above primitives, compiler support has been implemented for the following additional features.

| Compiler feature          | Example program                | Comments                                                        |
|-------------------------- |--------------------------------|-----------------------------------------------------------------|
| Secure memory-mapped I/O  | [sensor-reader](sensor-reader) | Assembly driver SM with exclusive ownership over sensor device. |
| Secure interrupt handling | [timer-protected](timer-protected) | Secure, authenticated interrupt delivery to an intra-SM interrupt handler.  |
| Dynamic loading           | [dynamic-loader](dynamic-loader) | SM binary image is dynamically linked and loaded by untrusted support software at runtime.  |
| Intra-SM multiplication   | [arithmetic](arithmetic)       | Dummy SMs performing secure (inlined) arithmetic operations.    |
| Reactive programming      | [auth-exec](auth-exec)  | Minimalist (non-contiki) program with `SM_INPUT/OUTPUT`        |


## Running the examples

First, make sure you have a working Sancus distribution. See the [sancus-main](https://github.com/sancus-tee/sancus-main) repository for detailed installation instructions.

All example binaries can be ran in either the Sancus Verilog simulator, or on an external FPGA. To build and run the example in `sancus-sim`, proceed as follows:

```bash
$ make sim
```

Alternatively, to upload the example to an FPGA using `sancus-loader`:

```bash
$ make load
```


