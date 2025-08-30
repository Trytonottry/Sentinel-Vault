#!/bin/bash
# scripts/run_sandbox.sh

echo "🧱 Запуск в изолированной песочнице..."

bwrap \
    --ro-bind /usr /usr \
    --bind data /data \
    --dev /dev \
    --ro-bind /etc/ssl /etc/ssl \
    --proc /proc \
    --chdir /data \
    --unshare-user \
    --unshare-pid \
    --unshare-ipc \
    --unshare-net \
    --die-with-parent \
    ./build/sentinel_vault