// fragments.h
#ifndef FRAGMENTS_H
#define FRAGMENTS_H

#include "crypto.h"

void fragment_and_encrypt_recursive(unsigned char *data, int data_len,
                                    const char *password, int depth);

#endif