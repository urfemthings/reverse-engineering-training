cat > cheatsheet.txt << 'EOF'
=== SEMUA COMMAND LEVEL 1-9 - REVERSE ENGINEERING ===

--- LEVEL 1-4 : STATIC ---
file ./levelX
strings ./levelX
checksec --file=./levelX
# Buka Ghidra -> cari main -> liat string

--- LEVEL 4-5 : XOR ---
python3 uni.py "TARGET" "KEY"
python3 uni.py "TARGET"
# contoh: python3 uni.py "7b 5a 2a 3d" "13 37 42 55"

--- LEVEL 5-6 : GDB AWAL ---
gdb ./latihan5
break main
run
x/18xb &target
x/3xb &key
info locals
info registers

# Rumus XOR Evolve:
# Level 6: key = key * 3 + 0x11
# Level 9: key = key * 7 + 0x15
# chr(byte ^ key)

--- LEVEL 7 : ANTI-DEBUG PTRACE ---
gdb ./latihan7v1
break ptrace
run
set $rax = 0
continue
disas main
x/17xb $rbp-0x30

--- LEVEL 8 : PIE + GDB LOGGING ---
set logging file hasil_gdb8.txt
set logging overwrite on
set logging on
set pagination off
file latihan8
starti
info proc mappings
break *BASE+OFFSET_GHIDRA
# contoh: break *0x555555555292
run
continue
x/43xb 0x555555558080

--- LEVEL 9 : LENGKAP - PIE + PTRACE + TRACERPID + XOR EVOLVE ---
# Ini yang kemarin belum lengkap!

# 1. Cek anti-debug dulu
cat /proc/self/status | grep TracerPid
# Kalo di GDB, TracerPid != 0 = kedetect

# 2. GDB Setup
gdb ./latihan9
set pagination off
set logging file hasil_gdb9.txt
set logging overwrite on
set logging on

# 3. Bypass ptrace pertama
break ptrace
run
set $rax = 0
continue

# 4. Cari BASE PIE
starti
info proc mappings
# Catat BASE, contoh: 0x555555554000

# 5. Break di main (BASE + OFFSET GHIDRA)
break *0x555555555292
continue

# 6. Di dalam main, bypass TracerPid check
# Cari cmp TracerPid / open /proc/self/status
# Kalo ketemu jne -> set $eflags biar lolos
# atau: set $rax = 0 pas baca TracerPid

# 7. Dump target & decrypt
x/43xb $rbp-0x30
# Atau pake alamat absolut:
x/43xb 0x555555558080

# 8. Decrypt di Python (Level 9)
# key awal dari Ghidra, rumus: key = key * 7 + 0x15
python3 -c "
data = [0x.., 0x..]  # isi dari x/43xb
key = 0x??  # key awal
out = ''
for b in data:
    out += chr(b ^ key)
    key = (key * 7 + 0x15) & 0xFF
print(out)
"

# Navigasi GDB
stepi   # 1 langkah, MASUK ke call
nexti   # 1 langkah, LONCATIN call
finish  # keluar fungsi
x/40i $pc
x/s $rdi
x/20xw $rsp

--- UNIVERSAL : BISA DIMANA AJA ---
gdb -batch -ex "break main" -ex "run" -ex "info locals" -ex "info registers" -ex "x/100xb $rsp" ./NAMA_FILE

--- ALUR UNIVERSAL ---
starti -> info proc mappings -> break *base+offset
-> stepi (bukan call) / nexti (kalo call)
-> x/40i $pc -> cari SUS -> break lagi

--- CARI SUS ---
cmp/test + jne = cek password
xor berulang = decrypt
call *%reg = shellcode
mmap/mprotect = bikin memori exec
ptrace/TracerPid/rdtsc = anti-debug

--- GITIGNORE BUAT GITHUB ---
*.out
*.o
*.so
hasil_gdb*.txt
latihan
latihan2b
latihan3
latihan4
latihan5
latihan6
latihan7v1
latihan8
latihan9
bypass
EOF
cat cheatsheet.txt
