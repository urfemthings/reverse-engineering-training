#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>
#include <unistd.h>

int main() {
    char input[100];
    // TARGET BENERAN (kalo gak di-debug)
    unsigned char target_real[] = {0x1d, 0x5b, 0x2c, 0x04, 0x72, 0x05, 0x19, 0x2c, 0x26, 0x2a, 0x88, 0x6e, 0x5e, 0x91, 0x59, 0x6c, 0x31};
    // TARGET PALSU (kalo ketauan di-debug)
    unsigned char target_fake[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00, 0x12};
    unsigned char *target = target_real;

    // INI ANTI-DEBUG-NYA (V1 - PTRACE)
    if (ptrace(PTRACE_TRACEME, 0, 0, 0) == -1) {
        printf("[!] Ketauan nge-debug! Nih TARGET PALSU!\n");
        target = target_fake;
    }

    printf("Masukin password: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    // Cek XOR + Evolve (sama kayak Level 6 lu)
    int key = 0x5a;
    int ok = 1;
    if (strlen(input) != 17) ok = 0;
    else {
        for (int i = 0; i < 17; i++) {
            if ((input[i] ^ key) != target[i]) { ok = 0; break; }
            key = (key * 3 + 0x11) & 0xFF;
        }
    }

    if (ok) printf("BENAR! GDB_Pro_Level7_OK\n");
    else printf("SALAH!\n");
    return 0;
}
