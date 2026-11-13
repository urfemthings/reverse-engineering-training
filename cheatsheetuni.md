cat > cheatsheet_gdb_universal.txt << 'EOF'
=== GDB UNIVERSAL CHEATSHEET + LEVEL 9 BARU ===

1. START
starti
info proc mappings  -> cari BASE (r-xp)
# BARU Level 9: kalo mappings kepanjangan
info proc mappings | grep r-xp

2. BREAK
break *BASE+GHIDRA
contoh: break *0x555555555352
kalo bukan PIE: break *0x401352 langsung

# BARU Level 9: Break di semua anti-debug sekaligus
break ptrace
break open
break fopen
break __isoc99_fscanf

3. JALAN
stepi = 1 langkah (bukan call)
nexti = loncatin call
continue = ke breakpoint berikutnya
finish = keluar dari fungsi
# BARU Level 9:
until *$pc+0x50 = jalan sampe alamat tertentu, tanpa break
stepi 20 = stepi 20x langsung
nexti 20 = nexti 20x langsung

4. LIAT MEMORI
x/20i $pc  = 20 assembly ke depan
x/s $rdi   = string di rdi
x/20xw $rsp = stack hex
x/20gx $rbp = data 8-byte
x/20xb $rax = byte mentah

# BARU Level 9 - CARI DI MEMORI (wajib karena PIE):
find 0x555555554000, +0x2000, "flag"
find 0x555555554000, +0x2000, 0x13, 0x37
# Cari string "TracerPid" / "/proc/self/status"
find $rsp, +0x1000, "/proc"

# BARU Level 9 - WATCHPOINT (berhenti pas memori berubah):
watch *0x555555558080
rwatch *0x555555558080
awatch *0x555555558080

# BARU Level 9 - LIAT SEMUA REGISTER + STACK SEKALIGUS:
info registers
info locals
info args
backtrace
x/40gx $rsp

5. CARI SUS
cmp/test + jne = cek password
xor berulang = decrypt
call *%reg = shellcode
mmap/mprotect = bikin memori exec
ptrace/TracerPid/rdtsc = anti-debug

6. LEVEL 9 - BYPASS LENGKAP (URUTAN BENER)
# Step A: Bypass ptrace
break ptrace
run
set $rax = 0
continue

# Step B: Bypass TracerPid
# Pas ketemu open("/proc/self/status") / fscanf TracerPid
# Liat di $rax hasil TracerPid, paksa jadi 0:
set $rax = 0
# Atau patch jne jadi je:
# Cari alamat jne-nya di x/20i $pc, lalu:
set *(char*)0x5555555553XX = 0x74  # 0x74=je, 0x75=jne
# Cara gampang: pas di cmp TracerPid, ubah flags:
set $eflags = 0x246

# Step C: Bypass rdtsc / timing check
# Kalo ada rdtsc + cmp lama, langsung:
set $rax = 0
set $rdx = 0

# Step D: Dump target
x/43xb $rbp-0x30
# Simpan ke file biar gak copy manual:
set logging file hasil_gdb9.txt
set logging on
x/43xb $rbp-0x30
set logging off

7. LEVEL 9 - PYTHON DECRYPT (BARU!)
# Jangan decrypt manual satu-satu, pake ini:
python3 << 'PY'
# Ganti data ini dari hasil x/43xb lu
data = [0x7b, 0x5a, 0x2a, 0x3d]  # contoh, isi 43 byte asli lu
key = 0x13  # key awal dari Ghidra, liat di main

out = ""
for b in data:
    out += chr(b ^ key)
    key = (key * 7 + 0x15) & 0xFF  # RUMUS LEVEL 9!
    print(f"{hex(b)} ^ {hex(key)} = {chr(b ^ key)}")
print("\nFLAG:", out)
PY

# PYTHON DI DALAM GDB (paling pro, gak usah keluar GDB):
# Ketik di dalam gdb:
python
data = [0x7b, 0x5a]
key = 0x13
for b in data:
    print(chr(b ^ key))
    key = (key * 7 + 0x15) & 0xFF
end

# PYTHON AUTO-DUMP DARI GDB:
python
import gdb
addr = int(gdb.parse_and_eval("$rbp-0x30"))
mem = gdb.selected_inferior().read_memory(addr, 43)
key = 0x13
flag = ""
for b in mem:
    flag += chr(b ^ key)
    key = (key * 7 + 0x15) & 0xFF
print("FLAG:", flag)
end

ALUR: starti -> mappings -> break -> stepi/nexti -> x/20i $pc -> break lagi
ALUR LEVEL 9: starti -> break ptrace (set rax=0) -> break TracerPid (set rax=0) -> dump -> python decrypt
EOF
cat cheatsheet_gdb_universal.txt
