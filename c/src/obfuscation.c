// src/obfuscation.c
#include "obfuscation.h"
#include "crypto.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

// Ключ обфускации (можно генерировать динамически)
static const unsigned char OBFUSCATION_KEY[32] = 
    "obfusc8k3y_s3cur3_v4ult";

// Простая XOR-обфускация с ротацией
void obfuscate_data(unsigned char *data, size_t len, const unsigned char *key) {
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key[i % 32];
        // Ротация байта
        data[i] = (data[i] << 3) | (data[i] >> 5);
    }
}

void deobfuscate_data(unsigned char *data, size_t len, const unsigned char *key) {
    for (size_t i = 0; i < len; i++) {
        // Обратная ротация
        data[i] = (data[i] >> 3) | (data[i] << 5);
        data[i] ^= key[i % 32];
    }
}

// Анти-отладка: проверка ptrace
int is_debugger_present() {
#ifdef __linux__
    if (ptrace(PTRACE_TRACEME, 0, NULL, 0) == -1) {
        return 1; // Уже отлаживается
    }
    ptrace(PTRACE_DETACH, 0, NULL, 0);
#endif

    // Проверка через /proc/self/status (TracerPid)
    FILE *f = fopen("/proc/self/status", "r");
    if (!f) return 0;

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "TracerPid:", 10) == 0) {
            int pid = atoi(line + 10);
            fclose(f);
            return pid > 0;
        }
    }
    fclose(f);
    return 0;
}

// Очистка памяти (надёжная)
void secure_clean(void *data, size_t len) {
    if (data) {
        explicit_bzero(data, len); // GNU, или используем volatile
    }
}

// Блокировка памяти (чтобы не выгружалась на диск)
void lock_memory_region(void *ptr, size_t len) {
#ifdef __linux__
    if (mlock(ptr, len) != 0) {
        perror("mlock failed");
    }
#endif
}