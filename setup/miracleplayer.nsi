; 此脚本用于打包奇迹音乐，仅供交流学习和非商业使用。

!include nsDialogs.nsh
!include MUI2.nsh
!include FileFunc.nsh
!include x64.nsh

Name "奇迹音乐"
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
!define MUI_FINISHPAGE_RUN_TEXT "启动$(^Name)"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH
  
!define MUI_LANGDLL_ALLLANGUAGES
!insertmacro MUI_LANGUAGE "SimpChinese"

BrandingText $(^Name)

VIProductVersion 1.0.0.1
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "ProductName" "奇迹音乐"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "ProductVersion" "1.0.0.1"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "FileDescription" "奇迹音乐安装程序"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "CompanyName" "奇迹音乐开发组"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "FileVersion" "1.1.0.0"
VIAddVersionKey /LANG=${LANG_SIMPCHINESE} "LegalCopyright" "版权所有"

InstType "只安装程序" PROGRAM_ONLY
InstType "完整安装" FULL

Section "播放器主程序" MAINCOMP
    SectionInstType ${PROGRAM_ONLY} ${FULL} RO
    DetailPrint "正在安装主程序文件"
    SetOutPath $INSTDIR
    WriteUninstaller ${UNINSTEXE}
    File /r "..\miracleplayer_release\*"
    ${GetSize} $INSTDIR "/S=0K" $0 $1 $2
    ${GetFileVersion} ${MAINEXE} $PRODUCT_VERSION
    SetShellVarContext all

    DetailPrint "正在创建快捷方式"
    !insertmacro MUI_STARTMENU_WRITE_BEGIN $(^Name)
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortcut "$SMPROGRAMS\$StartMenuFolder\$(^Name).lnk" ${MAINEXE}
    !insertmacro MUI_STARTMENU_WRITE_END
    ${If} $CreateDesktopShortcut <> 0
        CreateShortcut "$DESKTOP\$(^Name).lnk" ${MAINEXE}
    ${EndIf}

    DetailPrint "正在向注册表写入卸载信息"
    WriteRegStr HKLM "${PRODUCT_INSTALL_KEY}" "InstallLocation" $INSTDIR
    WriteRegStr HKLM "${PRODUCT_INSTALL_KEY}" "DisplayName" $(^Name)
    WriteRegStr HKLM "${PRODUCT_INSTALL_KEY}" "DisplayVersion" $PRODUCT_VERSION
    WriteRegStr HKLM "${PRODUCT_INSTALL_KEY}" "Publisher" "奇迹音乐开发组"
    WriteRegStr HKLM "${PRODUCT_INSTALL_KEY}" "DisplayIcon" ${MAINEXE}
    WriteRegStr HKLM "${PRODUCT_INSTALL_KEY}" "UninstallString" ${UNINSTEXE}
    WriteRegDWORD HKLM "${PRODUCT_INSTALL_KEY}" "EstimatedSize" $0

    WriteRegStr HKLM SOFTWARE\Classes\${ProductName} "" "音频文件"
    WriteRegStr HKLM SOFTWARE\Classes\${ProductName}\DefaultIcon "" ${MAINEXE}
    WriteRegStr HKLM SOFTWARE\Classes\${ProductName}\shell\open "" "使用奇迹音乐播放"
    WriteRegStr HKLM SOFTWARE\Classes\${ProductName}\shell\open\command "" '"${MAINEXE}" "%1"'
SectionEnd

LangString DESC_MAINCOMP ${LANG_SIMPCHINESE} "奇迹音乐播放器主程序，该项必须勾选。"

!macro FILEASSOC_REGISTRY assoc
    Section /o ${assoc}
        SectionInstType ${FULL}
        DetailPrint "正在创建${assoc}文件关联"
        WriteRegNone HKLM "SOFTWARE\Classes\Applications\${MAINEXENAME}\SupportedTypes" ${assoc}
        WriteRegNone HKLM "SOFTWARE\Classes\${assoc}\OpenWithProgids" ${ProductName}
    SectionEnd
!macroend

!macro FILEASSOC_REGISTRY_UNINSTALL assoc
    Section un${assoc}
        DeleteRegValue HKLM "SOFTWARE\Classes\${assoc}\OpenWithProgids" ${ProductName}
    SectionEnd
!macroend

SectionGroup "文件关联" FILEASSOC
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

LangString DESC_FILEASSOC ${LANG_SIMPCHINESE} "以后想要使用奇迹音乐打开的文件类型"

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
!insertmacro MUI_DESCRIPTION_TEXT ${MAINCOMP} $(DESC_MAINCOMP)
!insertmacro MUI_DESCRIPTION_TEXT ${FILEASSOC} $(DESC_FILEASSOC)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Section "Uninstall"
    SetShellVarContext all
    !insertmacro MUI_STARTMENU_GETFOLDER "$(^Name)" $StartMenuFolder

    DetailPrint "正在结束主程序进程"
    ExecWait "taskkill.exe /f /im MiraclePlayer.exe"

    DetailPrint "正在删除所有文件"
    RMDir /r "$SMPROGRAMS\$StartMenuFolder"
    Delete "$DESKTOP\$(^Name).lnk"
    RMDir /r "$INSTDIR"

    DetailPrint "正在删除卸载信息"
    DeleteRegKey HKLM "${PRODUCT_INSTALL_KEY}"

    MESSAGEBOX MB_YESNO|MB_ICONQUESTION "是否保留用户数据？" IDYES KeepUserData
    DetailPrint "正在删除用户数据"
    DeleteRegKey HKCU "Software\MiraclePlayerGroup"
    KeepUserData:
    
    DetailPrint "正在删除文件关联"
    DeleteRegKey HKLM "SOFTWARE\Classes\${ProductName}"
    DeleteRegKey HKLM "SOFTWARE\Classes\Applications\${MAINEXENAME}\SupportedTypes"
SectionEnd

Function ShowAdditionalTasks
	nsDialogs::Create 1018
    !insertmacro MUI_HEADER_TEXT "选择附加任务" "您想要安装程序执行哪些附加任务？"
    ${NSD_CreateLabel} 0u 0u 100% 13u "选择您想要安装程序执行的附加任务，然后单击[下一步(N)]继续。"
    ${NSD_CreateCheckbox} 0u 15u 100% 13u "创建桌面快捷方式"
    Pop $CreateDesktopShortcutCheckbox
    ${NSD_SetState} $CreateDesktopShortcutCheckbox $CreateDesktopShortcut
	nsDialogs::Show
FunctionEnd

Function LeaveAdditionalTasks
    ${NSD_GetState} $CreateDesktopShortcutCheckbox $CreateDesktopShortcut
FunctionEnd

Function .onInit
    ${IfNot} ${RunningX64}
        MESSAGEBOX MB_OK|MB_ICONEXCLAMATION "应用程序必须在64位计算机中安装！"
        Abort
    ${EndIf}

    StrCpy $CreateDesktopShortcut 1

    System::Call 'kernel32::CreateMutex(p 0, i 0, t "${ProductName}") p .r1 ?e'
    Pop $R0
    
    StrCmp $R0 0 +3
    MessageBox MB_OK|MB_ICONEXCLAMATION "已有一个安装实例正在运行。" 
    Abort
FunctionEnd
