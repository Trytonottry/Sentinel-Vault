#!/bin/bash
# setup.sh — One-Click Dependency Installer for Sentinel Vault
# Поддерживаемые ОС: Ubuntu/Debian, Fedora/RHEL, Arch

set -e

echo "🛡️  Установка зависимостей для Sentinel Vault..."
echo "OS: $(uname -s)"
echo "User: $(whoami)"

detect_distro() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        DISTRO=$NAME
    else
        echo "❌ Не удалось определить дистрибутив."
        exit 1
    fi
}

install_dependencies() {
    case "$DISTRO" in
        "Ubuntu"*|"Debian"*)
            sudo apt update
            sudo apt install -y \
                build-essential \
                libssl-dev \
                libglfw3-dev \
                libgles2-mesa-dev \
                pkg-config \
                cmake \
                git \
                wget \
                zip \
                unzip \
                curl \
                libgl1-mesa-dev \
                libxcursor-dev \
                libxinerama-dev \
                libxi-dev \
                libxrandr-dev \
                libxss-dev \
                libxtst-dev \
                libxcomposite-dev \
                libasound2-dev \
                libpulse-dev \
                libcunit1-dev \
                bubblewrap \
                appimagetool \
                tpm2-tools \
                tpm2-tss \
                mingw-w64 \
                nsis
            ;;

        "Fedora"*|"Red Hat"*|"CentOS"*)
            sudo dnf groupinstall -y "Development Tools"
            sudo dnf install -y \
                openssl-devel \
                glfw-devel \
                mesa-libGLES-devel \
                pkgconf-pkg-config \
                cmake \
                git \
                wget \
                zip \
                unzip \
                libXcursor-devel \
                libXinerama-devel \
                libXi-devel \
                libXrandr-devel \
                alsa-lib-devel \
                pulseaudio-libs-devel \
                CUnit-devel \
                bubblewrap \
                appimagekit \
                tpm2-tools \
                tpm2-tss \
                mingw64-gcc \
                mingw32-gcc \
                nsis
            ;;

        "Arch Linux"|"Manjaro Linux"|"EndeavourOS)
            sudo pacman -Sy --noconfirm \
                base-devel \
                openssl \
                glfw-x11 \
                gcc \
                pkgconf \
                cmake \
                git \
                wget \
                zip \
                unzip \
                xorgproto \
                libxcursor \
                libxinerama \
                libxi \
                libxrandr \
                alsa-lib \
                pulseaudio \
                cunit \
                bubblewrap \
                appimagetool \
                tpm2-tools \
                tpm2-tss \
                mingw-w64-gcc \
                nsis
            ;;

        *)
            echo "❌ Дистрибутив $DISTRO не поддерживается автоматически."
            echo "Пожалуйста, установите зависимости вручную."
            exit 1
            ;;
    esac
}

finalize() {
    echo ""
    echo "✅ Все зависимости установлены!"
    echo ""
    echo "🚀 Теперь вы можете собрать проект:"
    echo ""
    echo "   make full"
    echo "   ./SentinelVault-x86_64.AppImage"
    echo ""
    echo "💡 Для запуска в изолированной среде:"
    echo "   ./run_sandbox.sh"
    echo ""
    echo "🔐 TPM доступен, если у вас включён чип (проверьте: tpm2_pcrlist)"
}

# === Запуск ===
detect_distro
echo "🔍 Обнаружен дистрибутив: $DISTRO"
install_dependencies
finalize
