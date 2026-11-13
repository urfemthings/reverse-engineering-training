=================================================================
          GDB UNIVERSAL CHEATSHEET + LEVEL 9 (SMC & BYPASS)
=================================================================

[ 1. START & MAPPING ]
starti                              # Mulai & berhenti di entry point
info proc mappings                  # Cari alamat BASE (cari yang r-xp)
info proc mappings | grep r-xp      # Versi ringkas jika mappings panjang


[ 2. BREAKPOINT ]
break *BASE+OFFSET                  # Contoh: break *0x555555555352 (PIE)
break *0x401352                     # Jika non-PIE (alamat langsung)

# Break di fungsi anti-debug sekaligus:
break ptrace
break open
break fopen
break __isoc99_fscanf


[ 3. NAVIGASI EKSEKUSI ]
stepi                               # Jalan 1 instruksi (masuk ke dalam call)
nexti                               # Jalan 1 instruksi (lewati call)
stepi 20 / nexti 20                 # Jalan 20 instruksi sekaligus
until *$pc+0x50                     # Jalan sampai alamat tertentu tanpa break
finish                              # Selesaikan fungsi saat ini & keluar
continue                            # Lanjut sampai breakpoint berikutnya


[ 4. INSPEKSI MEMORI & REGISTER ]
x/20i $pc                           # Lihat 20 instruksi assembly ke depan
x/s $rdi                            # Baca string di register RDI
x/20xw $rsp                         # Lihat stack (hex 4-byte)
x/20gx $rbp                         # Lihat data di RBP (hex 8-byte)
x/20xb $rax                         # Lihat byte mentah di RAX
x/40gx $rsp                         # Lihat 40 baris stack

# Cek Register & Stack Frame:
info registers
info locals
info args
backtrace

# Cari Data di Memori (Wajib untuk PIE):
find 0x555555554000, +0x2000, "flag"
find 0x555555554000, +0x2000, 0x13, 0x37
find $rsp, +0x1000, "/proc"

# Watchpoint (Berhenti saat nilai memori berubah/dibaca):
watch *0x555555558080               # Berhenti saat ditulis (write)
rwatch *0x555555558080              # Berhenti saat dibaca (read)
awatch *0x555555558080              # Berhenti saat dibaca/ditulis


[ 5. POLA MENCURIGAKAN (SUS) DI ASSEMBLY ]
cmp / test + jne                    # Pengecekan password / kondisi
xor berulang                        # Routine dekripsi string / shellcode
call *%reg                          # Eksekusi shellcode dinamis
mmap / mprotect                     # Alokasi memori RWX (Self-Modifying Code)
ptrace / TracerPid / rdtsc          # Mekanisme anti-debugging


[ 6. LEVEL 9 - URUTAN BYPASS LENGKAP ]
# Step A: Bypass ptrace
break ptrace
run
set $rax = 0
continue

# Step B: Bypass TracerPid (/proc/self/status)
# Saat berhenti setelah pembacaan TracerPid, paksa hasil jadi 0:
set $rax = 0
# Opsi lain: Ubah EFLAGS saat di instruksi cmp (Zero Flag = 1):
set $eflags = 0x246
# Opsi lain: Patch instruksi jne (0x75) menjadi je (0x74):
set *(char*)0x5555555553XX = 0x74

# Step C: Bypass rdtsc / Timing Check
set $rax = 0
set $rdx = 0

# Step D: Dump Target & Simpan ke File Log
set logging file hasil_gdb9.txt
set logging on
x/43xb $rbp-0x30
set logging off


[ 7. LEVEL 9 - PYTHON DECRYPTOR ]
# --- Cara 1: Jalankan via Terminal Linux ---
python3 << 'PY'
data = [0x7b, 0x5a, 0x2a, 0x3d]  # Ganti dengan byte hasil dump GDB
key = 0x13                       # Key awal dari Ghidra/disassembly

out = ""
for b in data:
    dec = b ^ key
    out += chr(dec)
    print(f"{hex(b)} ^ {hex(key)} = {chr(dec)}")
    key = (key * 7 + 0x15) & 0xFF
print("\nFLAG:", out)
PY

# --- Cara 2: Auto-Dump & Decrypt Langsung di Dalam GDB ---
python
import gdb
addr = int(gdb.parse_and_eval("$rbp-0x30"))
mem = gdb.selected_inferior().read_memory(addr, 43)
key = 0x13
flag = ""
for b in mem:
    flag += chr(int.from_bytes(b, "little") ^ key)
    key = (key * 7 + 0x15) & 0xFF
print("FLAG:", flag)
end


=================================================================
RINGKASAN ALUR:
- Umum    : starti -> mappings -> break -> stepi/nexti -> x/20i $pc
- Level 9 : starti -> break ptrace (rax=0) -> break TracerPid (rax=0) -> dump -> python
=================================================================
