# Example: Programming a Sancus enclave in plain assembly

Expected output:

```
$ make sim 
sancus-cc    -c -o foo.o foo.S
sancus-ld -L"/usr/local/share/sancus-support"/lib/ --ram 16K --rom 41K  -lsm-io -ldev --inline-arithmetic --standalone --verbose -o no_mac_main.elf main.o foo.o
INFO: No new Sancus modules found
INFO: No existing Sancus modules found
INFO: No asm Sancus modules found
INFO: Found MSP430 install directory: /usr/msp430
INFO: Found MSP430 GCC install directory: /usr/lib/gcc/msp430/4.6.3
INFO: Using output file no_mac_main.elf
INFO: Cleaning up temporary files: /tmp/tmp7l3ng9qb
sancus-crypto --fill-macs --key 4078d505d82099ba --verbose -o main.elf no_mac_main.elf
sancus-sim  main.elf
2024-03-20 16:23:37.864 (   0.000s) [        89912740]               main.cpp:296   INFO| Using input file main.elf.
   v| 
INFO| arguments: sancus-sim main.elf
INFO| Current dir: /home/jo/Documents/sancus-main/sancus-examples/asm
INFO| stderr verbosity: 0
INFO| -----------------------------------
INFO| ======================= Sancus Simulator =======================
INFO| { Performing objcopy of elf file
INFO| .   Generating temporary binary form of given elf file...
INFO| .   Temp file is /tmp/tmp_sancus_gGU0yK
INFO| .   >> msp430-objcopy -O binary main.elf /tmp/tmp_sancus_gGU0yK
INFO| .   ..done!
INFO| } 0.002 s: Performing objcopy of elf file
INFO| Enabled automatic timeout after 1000000000 cycles.
INFO| Will abort simulation 0 cycles after any SM_VIOLATION
INFO| Read program memory of 41984 bytes.
=== SpongeWrap parameters ===
Rate:           16
Security:       64
Blocks in key:   4
=============================
=== Spongent parameters ===
Rate:        18
State size: 176
===========================

------


[main.c] enabling foo SM..
New SM 1 config: 5cae 5cbe 0296 0298, 0
Vendor key: 4078d505d82099ba
SM key: 14d0cb29714d1194
SM foo with ID 1 enabled	: 0x5cae 0x5cbe 0x0296 0x0298
[main.c] entering foo..
foo(0)=0xdead; foo(1)=0xbeef
[main.c] all done!



INFO| ======================== Simulation ended ========================
INFO| Total/crypto cycles simulated: 35004/2614.
INFO| ================ Simulation succeeded gracefully =================
INFO| atexit
```
