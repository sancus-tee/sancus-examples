import os

# build sm1.elf
os.system("sancus-cc -c -o sm1.o sm1.c")
os.system("sancus-ld --inline-arithmetic -o sm1.elf sm1.o")

# read ELF
with open("sm1.elf", "rb") as f:
    sm1 = f.read()

# prepare string to hardcode: remove brackets
sm1_array = str(list(sm1))[1:-1]

# read header template
with open("header_template.h", "r") as f:
    header = f.read()

# write sm1.h
with open("sm1.h", "w") as f:
    f.write(header.format(sm1_array))
