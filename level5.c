#include <stdio.h>
#include <string.h>

int main() {
    unsigned char key[] = {0x5a, 0x11, 0x33};
    unsigned char target[] = {0x1d, 0x79, 0x5a, 0x3e, 0x63, 0x52, 0x05, 0x5c, 0x52, 0x29, 0x65, 0x56, 0x28, 0x4e, 0x01, 0x6a, 0x23, 0x05};
    char input[100];
    printf("Masukin password: ");
    scanf("%99s", input);
    if (strlen(input) != 18) { printf("SALAH!\n"); return 0; }
    for (int i = 0; i < 18; i++) {
        if ((input[i] ^ key[i % 3]) != target[i]) {
            printf("SALAH!\n"); return 0;
        }
    }
    printf("BENAR! Flag dapet!\n");
    return 0;
}
