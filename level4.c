#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool check(char *pw) {
    if (strlen(pw) != 8) return false;
    int key[8] = {0x13, 0x37, 0x42, 0x55, 0x1A, 0x2B, 0x3C, 0x4D};
    int target[8] = {0x7B, 0x5A, 0x2A, 0x3D, 0x71, 0x4E, 0x15, 0x28};
    for (int i = 0; i < 8; i++) {
        if ((pw[i] ^ key[i]) != target[i]) return false;
    }
    return true;
}

int main() {
    char pw[100];
    printf("Masukin password: ");
    scanf("%s", pw);
    if (check(pw)) printf("BENAR! Flag dapet!\n");
    else printf("SALAH! Coba lagi.\n");
    return 0;
}
