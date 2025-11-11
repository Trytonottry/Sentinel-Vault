# 🛡️ Sentinel Vault — Система самозащиты данных

# Sentinel‑Vault

![Build Status](https://img.shields.io/github/actions/workflow/status/Trytonottry/Sentinel‑Vault/ci.yml?branch=main)
![Latest Release](https://img.shields.io/github/v/release/Trytonottry/Sentinel‑Vault)
![License](https://img.shields.io/github/license/Trytonottry/Sentinel‑Vault)
![Languages](https://img.shields.io/github/languages/top/Trytonottry/Sentinel‑Vault)
![Platform](https://img.shields.io/badge/platform‑Linux%20%7C%20Windows‑blue)
![GitHub stars](https://img.shields.io/github/stars/Trytonottry/Sentinel‑Vault?style=social)

> Высокозащищённое приложение с рекурсивным шифрованием, GUI, обфускацией, анти‑отладкой и самоуничтожением.


Высокозащищённое приложение с рекурсивным шифрованием, GUI, обфускацией, анти-отладкой и самоуничтожением.

## 🚀 Быстрый старт

```bash
git clone https://github.com/Trytonottry/Sentinel-Vault.git
cd sentinel_vault
./c/scripts/setup.sh
make full
./SentinelVault-x86_64.AppImage
```


## 🔧 Особенности 

    🔐 AES-256-GCM + PBKDF2
    🧩 Рекурсивное шифрование при вторжении
    🖥️ GUI (nuklear + GLFW)
    🛑 Анти-отладка и проверка целостности
    🐧 AppImage, 🪟 NSIS
    🧪 Unit-тесты
    🔐 OLLVM обфускация
    🧩 TPM 2.0
    🧱 Песочница (bubblewrap)

## ⚠️ Внимание 

    При запуске под gdb — самоуничтожение
    Изменение бинарника → отказ в запуске
    Пароль хранится только в зашифрованном виде
     

## 📦 Выходные файлы 

    SentinelVault-x86_64.AppImage — Linux
    sentinel_vault.exe — Windows
    build/sentinel_vault.o — обфусцированная версия
     
     
