# Sensor-reader SMs: An end-to-end example

This example program illustrates how to build and run a Sancus application. We
follow the example given in the paper: one protected module providing readings
from a memory-mapped I/O sensor, plus one that transforms this data and
wraps (encrypts + signs) it to be sent to the vendor.

The remote stakeholder is provided with an *authenticity* guarantee: a good
signature over sensor readings associated with a fresh nonce can only be
produced by untampered sensor/reader SMs. Moreover, the *confidentiality* of
the transformed sensor readings is preserved.

## Building and Running

Complete instructions are available on the [Sancus
website](https://distrinet.cs.kuleuven.be/software/sancus/examples.php). To
build the program, simulate the resulting binary, and verify its output, simply
run:

```bash
$ make clean sim
```
