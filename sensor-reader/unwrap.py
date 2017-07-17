#!/usr/bin/env python3
import re
import sys
import subprocess

me = sys.argv[0]

if (len(sys.argv) != 3):
    print('Usage: {} sm_key in_file'.format(me))
    sys.exit(1)

sm_key  = sys.argv[1]
sim_out = sys.argv[2]

def run_command(cmd):
    print('{}: {}'.format(me, ' '.join(cmd)))
    c = subprocess.run(cmd, stderr=subprocess.STDOUT, stdout=subprocess.PIPE)
    return (c.returncode, c.stdout)

RE_FMT = '.*{}.*is: (\w+)'

with open(sim_out) as f:
    for line in f:
        match = re.match(RE_FMT.format('Nonce'), line)
        if match:
            ad = match.group(1)
            continue

        match = re.match(RE_FMT.format('Cipher'), line)
        if match:
            cipher = match.group(1)
            continue

        match = re.match(RE_FMT.format('Tag'), line)
        if match:
            tag = match.group(1)
            continue

cmd = ['sancus-crypto', '--unwrap', ad, cipher, tag, '--key', sm_key]
(rv, out) = run_command(cmd)

if (not rv):
    print('{}: authentic sensor reading: {}'.format(me, out.hex()))
else:
    print('{}: fail: bad sensor reading: {}'.format(me, out.decode('utf-8')))

sys.exit(rv)
