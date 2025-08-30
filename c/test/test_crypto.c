// test/test_crypto.c
#include <CUnit/CUnit.h>
#include "../src/crypto.h"
#include <string.h>

void test_aes_encryption_decryption() {
    unsigned char key[32] = "testkey_32bytes________________";
    unsigned char iv[16] = "testiv_16bytes__";
    unsigned char plaintext[] = "Hello, World!";
    unsigned char ciphertext[128];
    unsigned char tag[16];
    unsigned char decrypted[128];

    int cipher_len = aes_encrypt(plaintext, strlen((char*)plaintext),
                                 key, iv, ciphertext, tag);
    CU_ASSERT_TRUE(cipher_len > 0);

    int plain_len = aes_decrypt(ciphertext, cipher_len, tag, key, iv, decrypted);
    decrypted[plain_len] = '\0';

    CU_ASSERT_STRING_EQUAL((char*)decrypted, (char*)plaintext);
}

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

int main() {
    CU_pSuite pSuite = NULL;

    if (CUnit_initialize() != CUE_SUCCESS) return CU_get_error();

    pSuite = CU_add_suite("Crypto Tests", init_suite, clean_suite);
    CU_add_test(pSuite, "AES Encrypt/Decrypt", test_aes_encryption_decryption);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CUnit_cleanup();
    return 0;
}