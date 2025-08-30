; installer/sentinel_vault.nsi
; NSIS Installer для Windows

OutFile "sentinel_vault_setup.exe"
InstallDir "$PROGRAMFILES\Sentinel Vault"

Page directory
Page instfiles

Section "Main"
    SetOutPath "$INSTDIR"
    File "build\sentinel_vault.exe"
    File "assets\license.txt"
    CreateShortcut "$SMPROGRAMS\Sentinel Vault.lnk" "$INSTDIR\sentinel_vault.exe"
SectionEnd

Section "Uninstall"
    Delete "$INSTDIR\*.*"
    Delete "$SMPROGRAMS\Sentinel Vault.lnk"
    RMDir "$INSTDIR"
SectionEnd

Function .onInit
    MessageBox MB_OKCANCEL "Запуск Sentinel Vault. Проверка целостности..." IDOK next
    Abort
next:
FunctionEnd