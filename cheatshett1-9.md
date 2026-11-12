=== SEMUA COMMAND LEVEL 1-9 - REVERSE ENGINEERING ===
Simpan sebagai: cheatsheet.txt

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

--- LEVEL 8-9 : PIE + ANTI-DEBUG LENGKAP ---
set logging file hasil_gdb.txt
set logging overwrite on
set logging on
set pagination off

file latihan9
starti
info proc mappings
break *BASE+OFFSET_GHIDRA
# contoh: break *0x555555555292
run
continue

stepi   # bukan call, 1 langkah
nexti   # kalo call, loncatin
finish  # keluar fungsi

x/40i $pc
x/s $rdi
x/20xw $rsp
x/43xb 0x555555558080

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
latihan*
hasil_gdb.txt
*.exe