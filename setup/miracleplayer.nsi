; �˽ű����ڴ���漣���֣���������ѧϰ�ͷ���ҵʹ�á�

!include nsDialogs.nsh
!include MUI2.nsh
!include FileFunc.nsh
!include x64.nsh

Name "�漣����"
OutFile "mplayer_setup_nsis.exe"
Var PRODUCT_VERSION
InstallDir "$PROGRAMFILES64\Miracle Player"
XPStyle on

!define ProductName MiraclePlayer
!define PRODUCT_INSTALL_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${ProductName}"
!define MAINEXENAME ${ProductName}.exe
!define MAINEXE $INSTDIR\${MAINEXENAME}
!define UNINSTEXE $INSTDIR\Uninstaller.exe

InstallDirRegKey HKLM "${PRODUCT_INSTALL_KEY}" "InstallLocation"

!define MUI_ICON "mplayer_inst.ico"
!define MUI_UNICON "mplayer_uninst.ico"
!define MUI_WELCOMEFINISHPAGE_BITMAP "welcomefinished.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "welcomefinished.bmp"
!define MUI_HEADERIMAGE
!define MUI_UNHEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "mplayer.bmp"
!define MUI_HEADERIMAGE_BITMAP_STRETCH AspectFitHeight
!define MUI_HEADERIMAGE_UNBITMAP_STRETCH AspectFitHeight
!define MUI_HEADERIMAGE_RIGHT
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\LICENSE"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
Var StartMenuFolder
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\MiraclePlayerGroup" 
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
!insertmacro MUI_PAGE_STARTMENU "$(^Name)" $StartMenuFolder
Var CreateDesktopShortcut
Var CreateDesktopShortcutCheckbox
Page custom ShowAdditionalTasks LeaveAdditionalTasks
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_RUN "$INSTDIR\MiraclePlayer.exe"
!define MUI_FINISHPAGE_RUN_TEXT "����$(^Name)"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH
  
!define MUI_LANGDLL_ALLLANGUAGES
!insertmacro MUI_LANGUAGE "SimpChinese"

BrandingText $(^Name)

VIProductVersion 1.0.0.1
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "ProductName" "�漣����"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "ProductVersion" "1.0.0.1"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "FileDescription" "�漣���ְ�װ����"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "CompanyName" "�漣���ֿ�����"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "FileVersion" "1.1.0.0"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "LegalCopyright" "��Ȩ����"

InstType "ֻ��װ����" PROGRAM_ONLY
InstType "������װ" FULL

Section "������������" MAINCOMP
    SectionInstType ${PROGRAM_ONLY} ${FULL} RO
    DetailPrint "���ڰ�װ�������ļ�"
    SetOutPath $INSTDIR
    WriteUninstaller ${UNINSTEXE}
    File /r "..\miracleplayer_release\*"
    ${GetSize} $INSTDIR "/S=0K" $0 $1 $2
    ${GetFileVersion} ${MAINEXE} $PRODUCT_VERSION
    SetShellVarContext all

    DetailPrint "���ڴ�����ݷ�ʽ"
    !insertmacro MUI_STARTMENU_WRITE_BEGIN $(^Name)
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortcut "$SMPROGRAMS\$StartMenuFolder\$(^Name).lnk" ${MAINEXE}
    !insertmacro MUI_STARTMENU_WRITE_END
    ${If} $CreateDesktopShortcut <> 0
        CreateShortcut "$DESKTOP\$(^Name).lnk" ${MAINEXE}
    ${EndIf}

    DetailPrint "������ע���д��ж����Ϣ"
    WriteRegStr HKLM "${PRODUCT_INSTALL_KEY}" "InstallLocation" $INSTDIR
    WriteRegStr HKLM "${PRODUCT_INSTALL_KEY}" "DisplayName" $(^Name)
    WriteRegStr HKLM "${PRODUCT_INSTALL_KEY}" "DisplayVersion" $PRODUCT_VERSION
    WriteRegStr HKLM "${PRODUCT_INSTALL_KEY}" "Publisher" "�漣���ֿ�����"
    WriteRegStr HKLM "${PRODUCT_INSTALL_KEY}" "DisplayIcon" ${MAINEXE}
    WriteRegStr HKLM "${PRODUCT_INSTALL_KEY}" "UninstallString" ${UNINSTEXE}
    WriteRegDWORD HKLM "${PRODUCT_INSTALL_KEY}" "EstimatedSize" $0

    WriteRegStr HKLM SOFTWARE\Classes\${ProductName} "" "��Ƶ�ļ�"
    WriteRegStr HKLM SOFTWARE\Classes\${ProductName}\DefaultIcon "" ${MAINEXE}
    WriteRegStr HKLM SOFTWARE\Classes\${ProductName}\shell\open "" "ʹ���漣���ֲ���"
    WriteRegStr HKLM SOFTWARE\Classes\${ProductName}\shell\open\command "" '"${MAINEXE}" "%1"'
SectionEnd

LangString DESC_MAINCOMP ${LANG_SIMPCHINESE} "�漣���ֲ����������򣬸�����빴ѡ��"

!macro FILEASSOC_REGISTRY assoc
    Section /o ${assoc}
        SectionInstType ${FULL}
        DetailPrint "���ڴ���${assoc}�ļ�����"
        WriteRegNone HKLM "SOFTWARE\Classes\Applications\${MAINEXENAME}\SupportedTypes" ${assoc}
        WriteRegNone HKLM "SOFTWARE\Classes\${assoc}\OpenWithProgids" ${ProductName}
    SectionEnd
!macroend

!macro FILEASSOC_REGISTRY_UNINSTALL assoc
    Section un${assoc}
        DeleteRegValue HKLM "SOFTWARE\Classes\${assoc}\OpenWithProgids" ${ProductName}
    SectionEnd
!macroend

SectionGroup "�ļ�����" FILEASSOC
    !insertmacro FILEASSOC_REGISTRY .mp3
    !insertmacro FILEASSOC_REGISTRY .wav
    !insertmacro FILEASSOC_REGISTRY .wma
    !insertmacro FILEASSOC_REGISTRY .mid
    !insertmacro FILEASSOC_REGISTRY .midi
    !insertmacro FILEASSOC_REGISTRY .ogg
    !insertmacro FILEASSOC_REGISTRY .aac
    !insertmacro FILEASSOC_REGISTRY .flac
    !insertmacro FILEASSOC_REGISTRY .m4a
SectionGroupEnd

!insertmacro FILEASSOC_REGISTRY_UNINSTALL .mp3
!insertmacro FILEASSOC_REGISTRY_UNINSTALL .wav
!insertmacro FILEASSOC_REGISTRY_UNINSTALL .wma
!insertmacro FILEASSOC_REGISTRY_UNINSTALL .mid
!insertmacro FILEASSOC_REGISTRY_UNINSTALL .midi
!insertmacro FILEASSOC_REGISTRY_UNINSTALL .ogg
!insertmacro FILEASSOC_REGISTRY_UNINSTALL .aac
!insertmacro FILEASSOC_REGISTRY_UNINSTALL .flac
!insertmacro FILEASSOC_REGISTRY_UNINSTALL .m4a

LangString DESC_FILEASSOC ${LANG_SIMPCHINESE} "�Ժ���Ҫʹ���漣���ִ򿪵��ļ�����"

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${MAINCOMP} $(DESC_MAINCOMP)
!insertmacro MUI_DESCRIPTION_TEXT ${FILEASSOC} $(DESC_FILEASSOC)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Section "Uninstall"
    SetShellVarContext all
    !insertmacro MUI_STARTMENU_GETFOLDER "$(^Name)" $StartMenuFolder

    DetailPrint "���ڽ������������"
    ExecWait "taskkill.exe /f /im MiraclePlayer.exe"

    DetailPrint "����ɾ�������ļ�"
    RMDir /r "$SMPROGRAMS\$StartMenuFolder"
    Delete "$DESKTOP\$(^Name).lnk"
    RMDir /r "$INSTDIR"

    DetailPrint "����ɾ��ж����Ϣ"
    DeleteRegKey HKLM "${PRODUCT_INSTALL_KEY}"

    MESSAGEBOX MB_YESNO|MB_ICONQUESTION "�Ƿ����û����ݣ�" IDYES KeepUserData
    DetailPrint "����ɾ���û�����"
    DeleteRegKey HKCU "Software\MiraclePlayerGroup"
    KeepUserData:
    
    DetailPrint "����ɾ���ļ�����"
    DeleteRegKey HKLM "SOFTWARE\Classes\${ProductName}"
    DeleteRegKey HKLM "SOFTWARE\Classes\Applications\${MAINEXENAME}\SupportedTypes"
SectionEnd

Function ShowAdditionalTasks
	nsDialogs::Create 1018
    !insertmacro MUI_HEADER_TEXT "ѡ�񸽼�����" "����Ҫ��װ����ִ����Щ��������"
    ${NSD_CreateLabel} 0u 0u 100% 13u "ѡ������Ҫ��װ����ִ�еĸ�������Ȼ�󵥻�[��һ��(N)]������"
    ${NSD_CreateCheckbox} 0u 15u 100% 13u "���������ݷ�ʽ"
    Pop $CreateDesktopShortcutCheckbox
    ${NSD_SetState} $CreateDesktopShortcutCheckbox $CreateDesktopShortcut
	nsDialogs::Show
FunctionEnd

Function LeaveAdditionalTasks
    ${NSD_GetState} $CreateDesktopShortcutCheckbox $CreateDesktopShortcut
FunctionEnd

Function .onInit
    ${IfNot} ${RunningX64}
        MESSAGEBOX MB_OK|MB_ICONEXCLAMATION "Ӧ�ó��������64λ������а�װ��"
        Abort
    ${EndIf}

    StrCpy $CreateDesktopShortcut 1

    System::Call 'kernel32::CreateMutex(p 0, i 0, t "${ProductName}") p .r1 ?e'
    Pop $R0
    
    StrCmp $R0 0 +3
    MessageBox MB_OK|MB_ICONEXCLAMATION "����һ����װʵ���������С�" 
    Abort
FunctionEnd
