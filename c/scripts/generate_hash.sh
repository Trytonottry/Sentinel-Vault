#!/bin/bash
# Генерирует хеш бинарника и вставляет в integrity.c
BINARY="build/sentinel_vault"
if [ ! -f "$BINARY" ]; then
    echo "Бинарник не найден: $BINARY"
    exit 1
fi

HASH=$(sha256sum "$BINARY" | awk '{print $1}')
echo "Сгенерирован хеш: $HASH"

# Преобразуем в массив C
C_HASH=$(echo $HASH | sed 's/\(..\)/0x\1,/g' | sed 's/,$//')

sed -i "s/static unsigned char KNOWN_HASH\[.*\] = {.*};/static unsigned char KNOWN_HASH\[32\] = {$C_HASH};/" src/integrity.c