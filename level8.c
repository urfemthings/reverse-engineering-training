#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_debugged() {
    FILE *f = fopen("/proc/self/status", "r");
    if (!f) return 0;
    char line[128];
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

int main() {
    char input[100];
    unsigned char target_real[] = {
        0x70, 0x72, 0x73, 0x47, 0xcb, 0x58, 0x9a, 0xb3,
        0xf3, 0xbb, 0x0f, 0xe5, 0xcf, 0x6a, 0xe2, 0x93, 0x00
    };
    unsigned char target_fake[] = {
        0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99
    };

    unsigned char *target = target_real;
    if (is_debugged()) {
        target = target_fake;
    }

    printf("Masukin password Level 8: ");
    if (!fgets(input, sizeof(input), stdin)) return 1;
    input[strcspn(input, "\r\n")] = 0;

    unsigned char key = 0x37;
    int ok = 1;
    if (strlen(input) != 17) {
        ok = 0;
    } else {
        for (int i = 0; i < 17; i++) {
            if (((unsigned char)input[i] ^ key) != target[i]) {
                ok = 0;
                break;
            }
            key = (key * 5 + 0x23) & 0xFF;
        }
    }

    if (ok) {
        printf("MANTAP! Flag Level 8 tembus!\n");
    } else {
        printf("SALAH!\n");
    }
    return 0;
}
