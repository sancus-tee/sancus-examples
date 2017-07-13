# Sensor-reader SMs: An end-to-end example

This example program illustrates how to build and run a Sancus application. We
follow the example given in the paper: one protected module providing sensor
data and one that transforms this data and signs it to be sent to the vendor. 

## Building and Running

Complete instructions are available on the [Sancus
website](https://distrinet.cs.kuleuven.be/software/sancus/examples.php). To
build the program, simulate the resulting binary, and verify its output, simply
run:

```bash
$ make SIM=1 clean sim verify
```
