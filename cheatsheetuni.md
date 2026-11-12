=== GDB UNIVERSAL CHEATSHEET ===

1. START
starti
info proc mappings  -> cari BASE (r-xp)

2. BREAK
break *BASE+GHIDRA
contoh: break *0x555555555352
kalo bukan PIE: break *0x401352 langsung

3. JALAN
stepi = 1 langkah (bukan call)
nexti = loncatin call
continue = ke breakpoint berikutnya
finish = keluar dari fungsi

4. LIAT MEMORI
x/20i $pc  = 20 assembly ke depan
x/s $rdi   = string di rdi
x/20xw $rsp = stack hex
x/20gx $rbp = data 8-byte
x/20xb $rax = byte mentah

5. CARI SUS
cmp/test + jne = cek password
xor berulang = decrypt
call *%reg = shellcode
mmap/mprotect = bikin memori exec
ptrace/TracerPid/rdtsc = anti-debug

ALUR: starti -> mappings -> break -> stepi/nexti -> x/20i $pc -> break lagi