# 🛡️ Sentinel Vault — Система самозащиты данных

# Sentinel‑Vault

![Build Status](https://img.shields.io/github/actions/workflow/status/Trytonottry/Sentinel‑Vault/ci.yml?branch=main)
![Latest Release](https://img.shields.io/github/v/release/Trytonottry/Sentinel‑Vault)
![License](https://img.shields.io/badge/MIT-green?style=for-the-badge)
![Languages](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Languages](https://img.shields.io/badge/Python-FFD43B?style=for-the-badge&logo=python&logoColor=blue)
![Platform](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![Platform](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)

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
     
     
