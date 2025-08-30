##!/bin/bash
# scripts/create_appimage.sh

APPDIR="SentinelVault.AppDir"
BINARY="sentinel_vault"

rm -rf $APPDIR
mkdir -p $APPDIR/usr/bin

cp build/$BINARY $APPDIR/usr/bin/

cat > $APPDIR/sentinel_vault.desktop << EOF
[Desktop Entry]
Name=Sentinel Vault
Exec=$BINARY
Icon=sentinel
Type=Application
Categories=Security;
EOF

cp assets/sentinel_logo.png $APPDIR/sentinel.png

cat > $APPDIR/AppRun << 'EOF'
#!/bin/sh
HERE="$(dirname "$(readlink -f "${0}")")"
export LD_LIBRARY_PATH="$HERE/usr/lib:$LD_LIBRARY_PATH"
exec "$HERE/usr/bin/sentinel_vault" "$@"
EOF

chmod +x $APPDIR/AppRun $APPDIR/usr/bin/$BINARY

# Скачиваем appimagetool, если нет
if ! command -v appimagetool &> /dev/null; then
    echo "⏬ Скачивание appimagetool..."
    wget -q https://github.com/AppImage/AppImageKit/releases/download/13/appimagetool-x86_64.AppImage
    chmod +x appimagetool-x86_64.AppImage
    ./appimagetool-x86_64.AppImage --appimage-extract
    mv squashfs-root appimagetool
    chmod +x appimagetool/AppRun
fi

./appimagetool/AppRun $APPDIR

echo "✅ AppImage создан: SentinelVault-x86_64.AppImage"