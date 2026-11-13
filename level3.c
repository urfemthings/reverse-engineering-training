#include <stdio.h>
#include <string.h>

int main() {
    volatile unsigned char data[] = {0x11, 0x1b, 0x17, 0x0f, 0x05, 0x12, 0x1f, 0x18, 0x1b, 0x0e, 0x00};
    volatile int kunci = 0x5A;
    char input[100];
    char asli[100];

    printf("Masukkan password rahasia: ");
    scanf("%s", input);

    for (int i = 0; data[i] != 0; i++) {
        asli[i] = data[i] ^ kunci;
    }
    asli[10] = 0;

    if (strcmp(input, asli) == 0) {
        printf("BOOM! Kamu nemu passwordnya!\n");
    } else {
        printf("Salah! Coba bongkar di Ghidra!\n");
    }
    return 0;
}
