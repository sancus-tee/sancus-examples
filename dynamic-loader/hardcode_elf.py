#!/usr/bin/python3

# read ELF
with open("sm1.elf", "rb") as f:
    sm1 = f.read()

# prepare string to hardcode: convert to byte array (without brackets)
sm1_array = str(list(sm1))[1:-1]

# read header template
with open("header_template.h", "r") as f:
    header = f.read()

# write sm1.h
with open("sm1.h", "w") as f:
    f.write(header.format(sm1_array))
