#include <stdio.h>

int cek_kode(int input) {
    int rahasia = 1234;
    if (input == rahasia) {
        return 1;
    }
    return 0;
}

int hitung_bonus(int nilai) {
    return nilai * 2 + 10;
}

int main() {
    int tebakan;
    printf("Masukkan kode: ");
    scanf("%d", &tebakan);

    if (cek_kode(tebakan)) {
        printf("Benar! Bonus kamu: %d\n", hitung_bonus(tebakan));
    } else {
        printf("Salah, coba lagi!\n");
    }
    return 0;
}
