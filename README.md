# 🛡️ Sentinel Vault — Система самозащиты данных

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
     
     
