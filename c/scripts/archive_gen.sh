#!/bin/bash
PROJECT_NAME="sentinel_vault_final"
rm -f ${PROJECT_NAME}.zip

zip -r ${PROJECT_NAME}.zip \
    src/ include/ lib/ assets/ \
    build/ data/ \
    Makefile CMakeLists.txt \
    README.md \
    scripts/ installer/ \
    test/ \
    create_appimage.sh \
    obfuscate_build.sh \
    run_sandbox.sh \
    generate_hash.sh

echo "📦 Архив готов: ${PROJECT_NAME}.zip"