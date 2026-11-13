#include <stdio.h>

int cek_kode(int kode) {
    volatile int kunci = 0x5A5A;
    volatile int target = 0x5E4E;
    return (kode ^ kunci) == target;
}

int hitung_bonus(int kode) {
    return (kode / 100) * 50 + (kode % 100) * 2;
}

int main() {
    int kode;
    printf("Masukkan kode: ");
    scanf("%d", &kode);
    if (cek_kode(kode)) {
        printf("Benar! Bonus kamu: %d\n", hitung_bonus(kode));
    } else {
        printf("Salah, coba lagi!\n");
    }
    return 0;
}
