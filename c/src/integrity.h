// src/integrity.h
#ifndef INTEGRITY_H
#define INTEGRITY_H

#include <openssl/sha.h>

int verify_binary_integrity(const char *binary_path);
void generate_binary_hash(const char *binary_path, unsigned char *out_hash);

#endif