#include <stdio.h>
#include <string.h>

int main() {
    unsigned char key = 0x5a;
    unsigned char target[] = {0x1d, 0x5b, 0x2c, 0x04, 0x72, 0x05, 0x19, 0x2c, 0x26, 0x2a, 0x88, 0x6e, 0x5e, 0x91, 0x59, 0x6c, 0x31};
    char input[100];
    printf("Masukin password: ");
    scanf("%99s", input);
    if (strlen(input) != 17) { printf("SALAH!\n"); return 0; }
    for (int i = 0; i < 17; i++) {
        if ((input[i] ^ key) != target[i]) { printf("SALAH!\n"); return 0; }
        key = (key * 3 + 0x11) & 0xFF;
    }
    printf("BENAR! Level 6 CLEAR!\n");
    return 0;
}
