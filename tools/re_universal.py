import sys
# cara pake: python3 re_universal.py "target hex" "key hex" xor / add / sub  evolve
# contoh Level 5: python3 re_universal.py "1d 79..." "5a 11 33" xor repeat
# contoh Level 6: python3 re_universal.py "1d 5b..." "5a" xor evolve

target = bytes.fromhex(sys.argv[1])
key = bytes.fromhex(sys.argv[2])
op = sys.argv[3] if len(sys.argv) > 3 else "xor"
mode = sys.argv[4] if len(sys.argv) > 4 else "repeat"

out = bytearray()
k = key[0]

for i, t in enumerate(target):
    if mode == "repeat":
        cur_key = key[i % len(key)]
    else: # evolve - rumus Level 6
        cur_key = k
        k = (k * 3 + 0x11) & 0xFF

    if op == "xor":
        out.append(t ^ cur_key)
    elif op == "add": # di C: input + key = target, jadi baliknya target - key
        out.append((t - cur_key) & 0xFF)
    elif op == "sub": # di C: input - key = target, jadi baliknya target + key
        out.append((t + cur_key) & 0xFF)

print(out.decode())
