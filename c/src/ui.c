// ui.c
#include "ui.h"
#include "crypto.h"
#include "trigger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show_menu() {
    printf("\n🔐 Secure Data Vault v0.1 (C)\n");
    printf("1. 🛠  Инициализировать данные\n");
    printf("2. 🔓 Попытаться получить доступ\n");
    printf("3. 🧹 Очистить всё\n");
    printf("4. 🕒 Запустить таймер саморазрушения (5 сек)\n");
    printf("5. 🚪 Выйти\n");
    printf("Выберите: ");
}

void handle_init(const char *password) {
    const char *data_file = "data/sensitive_data.bin";
    FILE *f = fopen(data_file, "rb");
    if (f) {
        printf("❌ Данные уже существуют.\n");
        fclose(f);
        return;
    }

    unsigned char original_data[] = "Secret: Galactic Defense Plan v3.14";
    int data_len = strlen((char*)original_data);

    unsigned char master_salt[SALT_LEN];
    unsigned char master_iv[IV_LEN];
    generate_salt_iv(master_salt, master_iv);

    unsigned char data_key[KEY_LEN];
    derive_key(password, master_salt, data_key);

    unsigned char *ciphertext = malloc(data_len + 16);
    unsigned char tag[16];
    int cipher_len = aes_encrypt(original_data, data_len, data_key, master_iv,
                                 ciphertext, tag);

    f = fopen(data_file, "wb");
    fwrite(master_salt, 1, SALT_LEN, f);
    fwrite(master_iv, 1, IV_LEN, f);
    fwrite(tag, 1, 16, f);
    fwrite(ciphertext, 1, cipher_len, f);
    fclose(f);

    free(ciphertext);
    printf("✅ Данные зашифрованы и сохранены.\n");
}

void handle_access(const char *password) {
    const char *data_file = "data/sensitive_data.bin";
    FILE *f = fopen(data_file, "rb");
    if (!f) {
        printf("❌ Файл данных не найден.\n");
        return;
    }

    if (is_intrusion_detected()) {
        printf("🚨 Вторжение обнаружено! Запуск фрагментации...\n");
        fseek(f, 0, SEEK_END);
        long len = ftell(f);
        fseek(f, 0, SEEK_SET);
        unsigned char *data = malloc(len);
        fread(data, 1, len, f);
        fclose(f);
        fragment_and_encrypt_recursive(data, len, password, 0);
        remove(data_file);
        return;
    }

    unsigned char master_salt[SALT_LEN];
    unsigned char master_iv[IV_LEN];
    unsigned char tag[16];
    fread(master_salt, 1, SALT_LEN, f);
    fread(master_iv, 1, IV_LEN, f);
    fread(tag, 1, 16, f);

    fseek(f, 0, SEEK_END);
    long cipher_len = ftell(f) - SALT_LEN - IV_LEN - 16;
    unsigned char *ciphertext = malloc(cipher_len);
    fread(ciphertext, 1, cipher_len, f);
    fclose(f);

    unsigned char data_key[KEY_LEN];
    derive_key(password, master_salt, data_key);

    unsigned char plaintext[256];
    int plain_len = aes_decrypt(ciphertext, cipher_len, tag, data_key, master_iv, plaintext);
    free(ciphertext);

    if (plain_len > 0) {
        plaintext[plain_len] = '\0';
        printf("🔓 Данные: %s\n", plaintext);
    } else {
        printf("❌ Ошибка расшифровки. Возможно, неверный пароль.\n");
        set_intrusion_trigger();
    }
}

void handle_cleanup() {
    system("rm -rf data/ fragments/ access_trigger.lock");
    mkdir("data", 0700);
    clear_intrusion_trigger();
    printf("🧹 Очистка завершена.\n");
}