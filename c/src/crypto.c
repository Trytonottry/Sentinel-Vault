// crypto.c
#include "crypto.h"

static void burn_stack(void *ptr, size_t size) {
    volatile unsigned char *p = (volatile unsigned char *)ptr;
    for (size_t i = 0; i < size; i++) p[i] = 0;
}

int derive_key(const char *password, const unsigned char *salt, unsigned char *key) {
    if (PKCS5_PBKDF2_HMAC(password, strlen(password),
                          salt, SALT_LEN,
                          100000, EVP_sha256(),
                          KEY_LEN, key) != 1) {
        return 0;
    }
    return 1;
}

int aes_encrypt(const unsigned char *plaintext, int plaintext_len,
                const unsigned char *key, const unsigned char *iv,
                unsigned char *ciphertext, unsigned char *tag) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return 0;

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL)) goto err;
    if (1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv)) goto err;

    int len;
    if (1 != EVP_EncryptUpdate(ctx, NULL, &len, NULL, plaintext_len)) goto err;

    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) goto err;
    int ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) goto err;
    ciphertext_len += len;

    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag)) goto err;

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;

err:
    EVP_CIPHER_CTX_free(ctx);
    return 0;
}

int aes_decrypt(const unsigned char *ciphertext, int ciphertext_len,
                const unsigned char *tag, const unsigned char *key,
                const unsigned char *iv, unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return 0;

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL)) goto err;
    if (1 != EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv)) goto err;
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, (void*)tag)) goto err;

    int len;
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) goto err;
    int plaintext_len = len;

    if (1 == EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) {
        plaintext_len += len;
        EVP_CIPHER_CTX_free(ctx);
        return plaintext_len;
    }

err:
    EVP_CIPHER_CTX_free(ctx);
    return 0;
}

void generate_salt_iv(unsigned char *salt, unsigned char *iv) {
    RAND_bytes(salt, SALT_LEN);
    RAND_bytes(iv, IV_LEN);
}