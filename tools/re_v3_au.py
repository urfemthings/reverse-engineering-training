# V3 FINAL - 25 RUMUS OTOMATIS
import base64, binascii

def ror(b, r):
    r %= 8
    return ((b >> r) | (b << (8-r))) & 0xFF if r else b

def rc4_decrypt(data, key=b"key"):
    S = list(range(256)); j=0
    for i in range(256):
        j = (j + S[i] + key[i % len(key)]) % 256
        S[i], S[j] = S[j], S[i]
    i=j=0; out=bytearray()
    for b in data:
        i=(i+1)%256; j=(j+S[i])%256
        S[i],S[j]=S[j],S[i]
        out.append(b ^ S[(S[i]+S[j])%256])
    return bytes(out)

def try_all(data, key_str="SECRET"):
    print("\n=== V3 AUTO - 25 RUMUS ===\n")
    def s(b): return ''.join(chr(x) if 32<=x<127 else '.' for x in b)
    print(f"1. XOR 0x13      : {s([b ^ 0x13 for b in data])}")
    k=0x13; o=[]
    for b in data: o.append(b ^ k); k=(k*7+0x15)&0xFF
    print(f"2. XOR Evolving  : {s(o)}")
    print(f"3. XOR Index     : {s([b ^ i for i,b in enumerate(data)])}")
    print(f"4. ADD -5        : {s([(b-5)&0xFF for b in data])}")
    print(f"5. SUB +3        : {s([(b+3)&0xFF for b in data])}")
    print(f"6. ADD - Index   : {s([(b-i)&0xFF for i,b in enumerate(data)])}")
    print(f"7. ROR 3         : {s([ror(b,3) for b in data])}")
    print(f"8. NOT           : {s([(~b)&0xFF for b in data])}")
    print(f"9. (enc-5)^0x13  : {s([((b-5)^0x13)&0xFF for b in data])}")
    print(f"10. Reverse      : {s(data[::-1])}")
    for kk in [b"SECRET", b"key", b"flag", b"1234", key_str.encode()]:
        print(f"11. XOR Key {kk} : {s([b ^ kk[i%len(kk)] for i,b in enumerate(data)])}")
    print(f"13. ROR i%8      : {s([ror(b, i%8) for i,b in enumerate(data)])}")
    print(f"14. XOR+ROL      : {s([ror(b ^ 0x13, 3) for b in data])}")
    print(f"15. (enc-i)^i    : {s([((b-i)^i)&0xFF for i,b in enumerate(data)])}")
    print(f"19. ROT13-ish    : {s([(b-13)&0xFF for b in data])}")
    print(f"20. Swap Nibble  : {s([((b&0x0F)<<4)|(b>>4) for b in data])}")
    for kk in [b"key", b"SECRET", b"password"]:
        print(f"21. RC4 Key {kk} : {s(rc4_decrypt(data, kk))}")
    try: print(f"17. Base64       : {base64.b64decode(bytes(data))}")
    except: pass
    print(f"18. Hex as text  : {s(bytes(data))}")
    print("\nCari yang jadi flag{...} / kata jelas!")

if __name__ == "__main__":
    s = input("Paste hex (ex: 7b 5a 2a): ")
    try_all([int(x,16) for x in s.split()])
