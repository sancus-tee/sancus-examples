expected output:

```
------


[untrusted] enabling ping/pong SMs..
SM ping with ID 1 enabled       : 0x7c04 0x833c 0x035a 0x04b4
SM pong with ID 2 enabled       : 0x742c 0x7c02 0x0200 0x035a

---
[untrusted] setting SM keys..
        key (128 bits) is: 83536bf3071fec8bc17cb216cac59a5e
        ad (48 bits) is: 000000000000
        cipher (128 bits) is: b9b2b8ad07a1b1a806c6a8445f8dcfe2
        tag (128 bits) is: 4028594b778efb0c65893aa389ada209
[untrusted] connection idx is 0
        key (128 bits) is: 83536bf3071fec8bc17cb216cac59a5e
        ad (48 bits) is: 000000000000
        cipher (128 bits) is: 6a005b980a0c538b07324cdb695dbe71
        tag (128 bits) is: 0e48ee86a7931d067ffb2f91ef170bec
[untrusted] connection idx is 0

---
[untrusted] calling ping SM..
[ping] producing auth output..
[untrusted] forwarding auth output from ping to pong..
        data (144 bits) is: 23508f6f7ccf78e1d75734cec8e2ef0d2e41
[pong] hi with auth input:
        data (16 bits) is: bbaa
[untrusted] pong returned 0
[untrusted] exiting..
```
