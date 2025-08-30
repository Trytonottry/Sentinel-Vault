// crypto.h
#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/hmac.h>
#include <openssl/kdf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LEN 32          // 256 bits
#define SALT_LEN 16
#define IV_LEN 16
#define MAX_RECURSION 5

// Генерация ключа из пароля (PBKDF2)
int derive_key(const char *password, const unsigned char *salt, unsigned char *key);

// AES-256-GCM шифрование
int aes_encrypt(const unsigned char *plaintext, int plaintext_len,
                const unsigned char *key, const unsigned char *iv,
                unsigned char *ciphertext, unsigned char *tag);

// AES-256-GCM расшифровка
int aes_decrypt(const unsigned char *ciphertext, int ciphertext_len,
                const unsigned char *tag, const unsigned char *key,
                const unsigned char *iv, unsigned char *plaintext);

// Генерация случайного IV и соли
void generate_salt_iv(unsigned char *salt, unsigned char *iv);

#endif