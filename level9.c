#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

int is_debugged() {
    FILE *f = fopen("/proc/self/status", "r");
    if (!f) return 0;
    char line[256];
    int pid = 0;
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "TracerPid:", 10) == 0) {
            pid = atoi(line + 10);
            break;
        }
    }
    fclose(f);
    return pid != 0;
}

unsigned char enc_code[] = {
    0x57, 0x48, 0x89, 0xfe, 0x56, 0x48, 0x89, 0xd6, 0x52, 0x31, 0xc0, 0x31, 0xd2,
    0x48, 0x31, 0xc9, 0x8a, 0x04, 0x0a, 0x84, 0xc0, 0x74, 0x12, 0x30, 0x0e, 0x88,
    0x04, 0x0a, 0x80, 0xc6, 0x0b, 0x80, 0xc1, 0x01, 0x80, 0xf9, 0x12, 0x75, 0xe8,
    0x5a, 0x5e, 0x5f, 0xc3
};

int main() {
    char input[100];
    struct timespec t1, t2;
    clock_gettime(CLOCK_MONOTONIC, &t1);

    printf("Masukin password Level 9: ");
    if (!fgets(input, sizeof(input), stdin)) return 1;
    input[strcspn(input, "\\r\\n")] = 0;

    clock_gettime(CLOCK_MONOTONIC, &t2);
    double elapsed = (t2.tv_sec - t1.tv_sec) + (t2.tv_nsec - t1.tv_nsec) / 1e9;
    if (elapsed > 3.0) {
        printf("[!] Terlalu lama (elapsed: %.2fs). Ketahuan nge-debug?\\n", elapsed);
        return 1;
    }

    if (is_debugged()) {
        printf("[!] TracerPid terdeteksi!\\n");
        return 1;
    }

    if (strlen(input) != 18) {
        printf("SALAH!\\n");
        return 0;
    }

    unsigned char key = 0x4a;
    for (int i = 0; i < sizeof(enc_code); i++) {
        enc_code[i] = (enc_code[i] ^ key) - 0x03;
        key = (key * 7 + 0x15) & 0xFF;
    }

    void *mem = mmap(NULL, sizeof(enc_code), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    memcpy(mem, enc_code, sizeof(enc_code));

    unsigned char target[] = {
        0x46, 0x42, 0x54, 0x5a, 0x57, 0x72, 0x65, 0x51, 0x51, 0x60, 0x67, 0x78,
        0x5f, 0x4c, 0x6c, 0x76, 0x7e, 0x59
    };

    void (*check_fn)(char *, unsigned char *) = (void (*)(char *, unsigned char *))((char *)mem);
    check_fn(input, target);

    if (memcmp(input, target, 18) == 0) {
        printf("GGWP! Kamu menembus Level 9 SMC!!!\\n");
    } else {
        printf("SALAH!\\n");
    }

    munmap(mem, sizeof(enc_code));
    return 0;
}
