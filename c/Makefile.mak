.PHONY: deps all test appimage win nsis obfuscate sandbox clean full

deps:
	@echo "🔧 Запуск setup.sh..."
	@chmod +x scripts/setup.sh
	@./scripts/setup.sh

all: build/sentinel_vault

build/sentinel_vault: $(wildcard src/*.c)
	@mkdir -p build
	@gcc -std=c99 -O2 -g -Wall -Wextra -Isrc -Iinclude \
		-D_GNU_SOURCE $(wildcard src/*.c) \
		-o $@ -lssl -lcrypto -lpthread -lglfw -lGL -lm

test: build/test_crypto
	./build/test_crypto

build/test_crypto: test/test_crypto.c src/crypto.c
	gcc -std=c99 -o $@ $^ -lcunit -lssl -lcrypto

appimage: all
	chmod +x scripts/create_appimage.sh
	sh scripts/create_appimage.sh

win: build/sentinel_vault.exe

build/sentinel_vault.exe: $(wildcard src/*.c)
	i686-w64-mingw32-gcc -std=c99 -O2 -Isrc \
		$(wildcard src/*.c) -o $@ \
		-lssl -lcrypto -lgdi32 -lws2_32

nsis: win
	makensis installer/sentinel_vault.nsi

obfuscate:
	chmod +x scripts/obfuscate_build.sh
	sh scripts/obfuscate_build.sh

sandbox:
	chmod +x scripts/run_sandbox.sh
	sh scripts/run_sandbox.sh

clean:
	rm -rf build/ data/fragments/ data/*.bin access_trigger.lock *.AppImage *.exe
	find . -name "*.o" -delete

full: clean deps all test appimage win nsis obfuscate
	@echo "🎉 Sentinel Vault полностью собран!"
	@echo "📦 AppImage: SentinelVault-x86_64.AppImage"
	@echo "💻 Windows: sentinel_vault.exe"
	@echo "🔐 Обфусцированная версия: build/sentinel_vault.o"