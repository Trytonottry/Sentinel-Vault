// src/obfuscation.h
#ifndef OBFUSCATION_H
#define OBFUSCATION_H

#include <stdio.h>
#include <string.h>
#include <sys/prctl.h>

// Включаем только на Linux
#ifdef __linux__
#include <sys/ptrace.h>
#endif

// Функции
void obfuscate_data(unsigned char *data, size_t len, const unsigned char *key);
void deobfuscate_data(unsigned char *data, size_t len, const unsigned char *key);
int is_debugger_present();
void secure_clean(void *data, size_t len);
void lock_memory_region(void *ptr, size_t len);

#endif