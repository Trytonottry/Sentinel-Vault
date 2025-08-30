import os
import hashlib
import secrets
from cryptography.fernet import Fernet
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC
import base64
import threading
import time
import shutil
import sys

# === Настройки ===
DATA_FILE = "sensitive_data.bin"
FRAGMENTS_DIR = "fragments"
KEY_FILE = "master.key"
MAX_RECURSION = 5  # Ограничение рекурсии, чтобы не уйти в бесконечность
SALT_FILE = "salt.bin"
TRIGGER_FILE = "access_trigger.lock"  # Файл-индикатор попытки доступа

# === Генерация ключа из пароля ===
def generate_key_from_password(password: str, salt: bytes) -> bytes:
    kdf = PBKDF2HMAC(
        algorithm=hashes.SHA256(),
        length=32,
        salt=salt,
        iterations=100_000,
    )
    key = base64.urlsafe_b64encode(kdf.derive(password.encode()))
    return key

# === Сохранение/чтение соли ===
def save_salt(salt: bytes):
    with open(SALT_FILE, "wb") as f:
        f.write(salt)

def load_salt() -> bytes:
    if os.path.exists(SALT_FILE):
        with open(SALT_FILE, "rb") as f:
            return f.read()
    else:
        salt = os.urandom(16)
        save_salt(salt)
        return salt

# === Инициализация ===
def initialize(password: str):
    if os.path.exists(DATA_FILE):
        print("Данные уже существуют.")
        return

    # Создаём тестовые данные
    data = b"Секретная информация: планы по защите галактики!"
    salt = load_salt()
    key = generate_key_from_password(password, salt)
    fernet = Fernet(Fernet.generate_key())  # Ключ для данных

    # Шифруем данные
    encrypted_data = fernet.encrypt(data)

    # Сохраняем ключ шифрования, зашифрованный паролем
    encrypted_key = Fernet(key).encrypt(fernet._encryption_key)

    with open(DATA_FILE, "wb") as f:
        f.write(encrypted_key + b"|||" + encrypted_data)

    print("✅ Данные инициализированы и зашифрованы.")

# === Фрагментация и рекурсивное шифрование ===
def fragment_and_encrypt( bytes, password: str, depth: int = 0):
    if depth >= MAX_RECURSION:
        print(f"🛑 Достигнута максимальная глубина рекурсии: {depth}")
        return

    print(f"🔁 Фрагментация и шифрование (уровень {depth})...")

    # Создаём директорию для фрагментов
    frag_dir = os.path.join(FRAGMENTS_DIR, f"level_{depth}")
    os.makedirs(frag_dir, exist_ok=True)

    # Разбиваем данные на 3 фрагмента
    chunk_size = len(data) // 3 + 1
    fragments = [data[i:i+chunk_size] for i in range(0, len(data), chunk_size)]

    salt = load_salt()
    key = generate_key_from_password(password, salt)
    fernet = Fernet(Fernet.generate_key())

    encrypted_fragments = []
    for i, frag in enumerate(fragments):
        # Каждый фрагмент шифруется отдельно
        encrypted_frag = fernet.encrypt(frag)
        frag_path = os.path.join(frag_dir, f"frag_{i}.enc")
        with open(frag_path, "wb") as f:
            f.write(encrypted_frag)
        encrypted_fragments.append(encrypted_frag)

    # Сохраняем зашифрованный ключ шифрования фрагментов
    encrypted_key = Fernet(key).encrypt(fernet._encryption_key)
    with open(os.path.join(frag_dir, "key.enc"), "wb") as kf:
        kf.write(encrypted_key)

    print(f"💾 Сохранено {len(fragments)} фрагментов на уровне {depth}")

    # Рекурсивно шифруем каждый фрагмент (симуляция бесконечности)
    for i, frag in enumerate(encrypted_fragments):
        fragment_and_encrypt(frag, password, depth + 1)

# === Проверка на "вторжение" ===
def is_intrusion_attempt():
    # Простой триггер: если кто-то пытается прочитать файл без разрешения
    if os.path.exists(TRIGGER_FILE):
        return True
    return False

def set_intrusion_trigger():
    with open(TRIGGER_FILE, "w") as f:
        f.write("INTRUSION DETECTED AT: " + str(time.time()))

# === Попытка доступа к данным ===
def access_data(password: str):
    if not os.path.exists(DATA_FILE):
        print("Файл данных не найден.")
        return

    # Проверяем, не было ли вторжения
    if is_intrusion_attempt():
        print("🚨 Обнаружена попытка несанкционированного доступа!")
        print("💥 Запуск рекурсивного разбиения и шифрования...")

        with open(DATA_FILE, "rb") as f:
            raw = f.read()
        # Удаляем оригинальные данные
        os.remove(DATA_FILE)
        # Запускаем защиту в отдельном потоке
        thread = threading.Thread(target=fragment_and_encrypt, args=(raw, password, 0))
        thread.start()
        return

    print("🔓 Попытка доступа...")
    try:
        with open(DATA_FILE, "rb") as f:
            parts = f.read().split(b"|||")
            if len(parts) != 2:
                raise ValueError("Неверный формат данных")

            encrypted_key, encrypted_data = parts

        salt = load_salt()
        key = generate_key_from_password(password, salt)
        # Расшифровываем ключ
        fernet_master = Fernet(key)
        data_key = fernet_master.decrypt(encrypted_key)
        fernet_data = Fernet(data_key)

        decrypted_data = fernet_data.decrypt(encrypted_data)
        print("🔐 Данные успешно расшифрованы:")
        print(decrypted_data.decode('utf-8', errors='replace'))

    except Exception as e:
        print(f"❌ Ошибка доступа: {e}")
        # Симуляция вторжения при неправильном пароле
        set_intrusion_trigger()
        print("Триггер вторжения установлен.")

# === Очистка ===
def cleanup():
    paths = [DATA_FILE, SALT_FILE, TRIGGER_FILE, FRAGMENTS_DIR, KEY_FILE]
    for path in paths:
        if os.path.exists(path):
            if os.path.isdir(path):
                shutil.rmtree(path)
            else:
                os.remove(path)
    print("🧹 Всё очищено.")

# === Главное меню ===
def main():
    print("🔐 Система самозащиты данных v0.1")
    print("1. Инициализировать данные")
    print("2. Попытаться получить доступ")
    print("3. Очистить всё")
    print("4. Выйти")

    password = "supersecret123"  # В реальности — ввод с клавиатуры

    while True:
        choice = input("\nВыберите действие: ")
        if choice == "1":
            initialize(password)
        elif choice == "2":
            access_data(password)
        elif choice == "3":
            cleanup()
        elif choice == "4":
            break
        else:
            print("Неверный выбор.")

if __name__ == "__main__":
    main()