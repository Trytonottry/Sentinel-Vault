// src/integrity.c
#include "integrity.h"
#include <stdio.h>
#include <openssl/sha.h>

// Хеш самого бинарника (заполняется при сборке)
static unsigned char KNOWN_HASH[SHA256_DIGEST_LENGTH] = {0};

int verify_binary_integrity(const char *binary_path) {
    unsigned char current_hash[SHA256_DIGEST_LENGTH];
    generate_binary_hash(binary_path, current_hash);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        if (current_hash[i] != KNOWN_HASH[i]) {
            return 0;
        }
    }
    return 1;
}

void generate_binary_hash(const char *binary_path, unsigned char *out_hash) {
    FILE *f = fopen(binary_path, "rb");
    if (!f) return;

    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    unsigned char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), f)) > 0) {
        SHA256_Update(&sha256, buffer, bytes);
    }
    fclose(f);

    SHA256_Final(out_hash, &sha256);
}