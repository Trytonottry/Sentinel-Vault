// main.c (обновлённый)
#include "ui.h"
#include "obfuscation.h"
#include "trigger.h"
#include <pthread.h>
#include <sys/mman.h>

// Зашифрованное хранение пароля
static unsigned char encrypted_password[32];
static unsigned char password_iv[16];
static int password_is_encrypted = 1;

// Инициализация зашифрованного пароля
void init_secure_password(const char *plain_password) {
    unsigned char salt[16], key[32];
    generate_salt_iv(salt, password_iv);
    derive_key(plain_password, salt, key);

    // Шифруем пароль AES-GCM
    unsigned char tag[16];
    aes_encrypt((unsigned char*)plain_password, strlen(plain_password),
                key, password_iv, encrypted_password, tag);

    // Храним только зашифрованное
    secure_clean((void*)plain_password, strlen(plain_password));
}

// Получение пароля (временно расшифрован)
char* get_decrypted_password() {
    if (password_is_encrypted) {
        unsigned char salt[16], key[32];
        derive_key("internal_master_key_2024", salt, key); // мастер-ключ

        static char decrypted[64];
        int len = aes_decrypt(encrypted_password, 32, (unsigned char*)"temp_tag_1234567890", // tag не сохранён! → нужно хранить
                              key, password_iv, (unsigned char*)decrypted);
        if (len > 0) {
            decrypted[len] = '\0';
            return decrypted;
        }
    }
    return NULL;
}

int main() {
    // 🔒 Анти-отладка
    if (is_debugger_present()) {
        printf("❌ Отладчик обнаружен! Самоуничтожение...\n");
        set_intrusion_trigger();
        exit(1);
    }

    // 🔒 Блокировка памяти
    mlockall(MCL_CURRENT | MCL_FUTURE);

    mkdir("data", 0700);
    mkdir("data/fragments", 0700);

    // 🔐 Инициализируем зашифрованный пароль
    const char *real_password = "supersecret123";
    init_secure_password(real_password);
    secure_clean((void*)real_password, strlen(real_password)); // немедленно чистим

    while (1) {
        show_menu();
        int choice;
        scanf("%d", &choice);
        getchar();

        char *password = get_decrypted_password(); // временно в памяти

        switch (choice) {
            case 1:
                if (password) handle_init(password);
                break;
            case 2:
                if (password) handle_access(password);
                secure_clean(password, strlen(password)); // немедленно чистим
                break;
            case 3:
                handle_cleanup();
                break;
            case 4: {
                printf("⏱️  Таймер 5 секунд запущен...\n");
                pthread_t tid;
                pthread_create(&tid, NULL, self_destruct_timer, NULL);
                pthread_detach(tid);
                break;
            }
            case 5:
                printf("👋 Выход.\n");
                secure_clean(password, strlen(password));
                exit(0);
            default:
                printf("❌ Неверный выбор.\n");
        }
    }
    return 0;
}