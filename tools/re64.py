# uni.py - UNIVERSAL SOLVER, BISA DIMANA AJA
# Level 1 sampe Level 1000, CTF apapun, bisa.
import sys, re, string

def parse(s):
    # Bisa baca format apapun: "7b 5a", "0x7b, 0x5a", "7b5a", dll
    return [int(x,16) for x in re.findall(r"(?:0x)?([0-9a-fA-F]{2})", s)]

def decode(t, k):
    # Key ngulang otomatis, panjang berapapun bisa
    return "".join(chr(b ^ k[i % len(k)]) for i,b in enumerate(t))

# CARA 1: Kalo lu udah tau TARGET + KEY (dari Ghidra / GDB)
# python3 uni.py "TARGET" "KEY"
if len(sys.argv) == 3:
    print(decode(parse(sys.argv[1]), parse(sys.argv[2])))

# CARA 2: Kalo lu CUMA tau TARGET, KEY-nya gak tau (Level misterius)
# python3 uni.py "TARGET"
# Dia bakal brute force 256 key, cari yang hasilnya bisa dibaca
elif len(sys.argv) == 2:
    t = parse(sys.argv[1])
    print("--- BRUTE FORCE KEY, CARI YANG MASUK AKAL ---")
    for key in range(256):
        pw = "".join(chr(b ^ key) for b in t)
        # Cuma tampilin yang isinya huruf/angka/_ yang bisa dibaca
        if all(c in string.printable and c not in "\x0b\x0c" for c in pw) and sum(c.isalnum() or c=='_' for c in pw) > len(pw)*0.8:
            print(f"key=0x{key:02x} ({key}) -> {pw}")
else:
    print('Pake: python3 uni.py "TARGET" "KEY"  atau  python3 uni.py "TARGET"')
