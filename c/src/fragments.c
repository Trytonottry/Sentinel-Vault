// fragments.c
#include "fragments.h"
#include "crypto.h"
#include "trigger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char* make_dir_path(const char *base, int depth) {
    char *path = malloc(256);
    snprintf(path, 256, "%s/level_%d", base, depth);
    mkdir(path, 0700);
    return path;
}

char* make_file_path(const char *dir, const char *name) {
    char *path = malloc(256);
    snprintf(path, 256, "%s/%s", dir, name);
    return path;
}

void fragment_and_encrypt_recursive(unsigned char *data, int data_len,
                                    const char *password, int depth) {
    if (depth >= MAX_RECURSION) {
        printf("🛑 Recursion limit reached: %d\n", depth);
        free(data);
        return;
    }

    printf("🔁 Fragmenting & encrypting (depth %d)...\n", depth);

    const char *frag_dir = make_dir_path("data/fragments", depth);
    mkdir("data/fragments", 0700); // root dir

    // Разбиваем на 3 части
    int chunk_size = data_len / 3 + 1;
    unsigned char *fragments[3];
    int sizes[3] = {0};

    for (int i = 0; i < 3; i++) {
        int start = i * chunk_size;
        int size = (start >= data_len) ? 0 : (data_len - start < chunk_size ? data_len - start : chunk_size);
        if (size > 0) {
            fragments[i] = malloc(size);
            memcpy(fragments[i], data + start, size);
            sizes[i] = size;
        } else {
            fragments[i] = NULL;
            sizes[i] = 0;
        }
    }
    free(data); // освобождаем исходный буфер

    // Ключ для шифрования фрагментов
    unsigned char frag_key[KEY_LEN];
    unsigned char frag_iv[IV_LEN];
    unsigned char frag_salt[SALT_LEN];
    generate_salt_iv(frag_salt, frag_iv);
    derive_key(password, frag_salt, frag_key);

    for (int i = 0; i < 3; i++) {
        if (sizes[i] == 0) continue;

        unsigned char *ciphertext = malloc(sizes[i] + 16); // GCM добавляет 16 байт
        unsigned char tag[16];

        int cipher_len = aes_encrypt(fragments[i], sizes[i], frag_key, frag_iv,
                                     ciphertext, tag);
        if (cipher_len <= 0) {
            printf("❌ Encryption failed for fragment %d\n", i);
            free(ciphertext);
            continue;
        }

        char filename[32];
        snprintf(filename, 32, "frag_%d.enc", i);
        char *filepath = make_file_path(frag_dir, filename);

        FILE *f = fopen(filepath, "wb");
        fwrite(frag_salt, 1, SALT_LEN, f);
        fwrite(frag_iv, 1, IV_LEN, f);
        fwrite(tag, 1, 16, f);
        fwrite(ciphertext, 1, cipher_len, f);
        fclose(f);

        free(fragments[i]);
        free(ciphertext);
        free(filepath);

        // Рекурсивный вызов
        unsigned char *next_data = malloc(cipher_len);
        memcpy(next_data, ciphertext, cipher_len);
        fragment_and_encrypt_recursive(next_data, cipher_len, password, depth + 1);
    }

    // Сохраняем ключ шифрования
    unsigned char master_key[KEY_LEN];
    unsigned char master_salt[SALT_LEN];
    unsigned char master_iv[IV_LEN];
    generate_salt_iv(master_salt, master_iv);
    derive_key(password, master_salt, master_key);

    unsigned char encrypted_key[KEY_LEN + 16];
    unsigned char key_tag[16];
    int key_len = aes_encrypt(frag_key, KEY_LEN, master_key, master_iv,
                              encrypted_key, key_tag);

    char *keypath = make_file_path(frag_dir, "key.enc");
    FILE *kf = fopen(keypath, "wb");
    fwrite(master_salt, 1, SALT_LEN, kf);
    fwrite(master_iv, 1, IV_LEN, kf);
    fwrite(key_tag, 1, 16, kf);
    fwrite(encrypted_key, 1, key_len, kf);
    fclose(kf);

    free(keypath);
    free(frag_dir);
}