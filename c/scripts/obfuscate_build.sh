#!/bin/bash
# scripts/obfuscate_build.sh

echo "🔐 Запуск обфускации через OLLVM..."

# Проверка Docker
if ! command -v docker &> /dev/null; then
    echo "❌ Docker не установлен. Установите: https://docs.docker.com/get-docker/"
    exit 1
fi

# Обфусцируем
docker run --rm -v $(pwd):/work -w /work yegorov/obfuscator-llvm \
    clang -m64 -O2 -fno-stack-protector \
    -Xclang -load -Xclang /opt/obfuscator/lib/llvm-obsf.so \
    -fla -sbc -sub -bcf \
    src/*.c -o build/sentinel_vault.o \
    -Isrc -lssl -lcrypto -lpthread -lglfw -lGL -lm

echo "✅ Обфусцированный бинарник: build/sentinel_vault.o"