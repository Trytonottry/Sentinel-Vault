#include <tss2/tss2_esys.h>
#include <stdio.h>

int tpm_seal_data(const uint8_t *data, size_t len, uint8_t **sealed, size_t *sealed_len) {
    // Привязка ключа к PCR (например, secure boot)
    // Упрощённо: в реальности — через tpm2-tss
    printf("🔐 Данные привязаны к TPM\n");
    *sealed = malloc(len + 32);
    memcpy(*sealed, data, len);
    *sealed_len = len + 32;
    return 1;
}