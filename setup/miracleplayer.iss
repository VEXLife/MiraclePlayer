#define MyAppName "�漣����"
#define MyAppVersion "1.0.0.1"
#define MyAppPublisher "�漣���ֿ�����"
#define MyAppURL "https://gitee.com/vexlife/miracle-player"
#define MyAppExeName "MiraclePlayer.exe"
#define MyAppAssocName "��Ƶ�ļ�"
#define MyAppAssocKey "MiraclePlayer"

#define ComponentAssoc(assoc) "Name: ""fileassoc\" + assoc + """; Description: ""." + assoc + "��ʽ""; Types: full;"
#define InstallAssoc(assoc) "Root: HKA; Subkey: ""Software\Classes\." + assoc + "\OpenWithProgids""; ValueType: binary; ValueName: " + MyAppAssocKey + "; ValueData: """"; Components: ""fileassoc\" + assoc + """; Flags: uninsdeletevalue;" + NewLine + \
"Root: HKA; Subkey: ""Software\Classes\Applications\" + MyAppExeName + "\SupportedTypes""; ValueType: binary; ValueName: ""." + assoc + """; ValueData: """"; Components: ""fileassoc\" + assoc + """; Flags: uninsdeletekey"

[Setup]
AppId={{C10266F4-6C34-4981-ADB6-804D35EF2F8A}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName}������
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64   
CloseApplications=yes
DefaultDirName={autopf}\Miracle Player
DisableWelcomePage=no
ChangesAssociations=yes
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
LicenseFile=..\LICENSE
OutputDir=.
OutputBaseFilename=mplayer_setup_iss
SetupIconFile=mplayer_inst.ico
Compression=lzma
SolidCompression=yes            
UninstallDisplayIcon={app}\{#MyAppExeName}
UninstallDisplayName={#MyAppName}
WizardImageFile=welcomefinished.bmp
WizardSmallImageFile=mplayer.bmp
WizardStyle=modern

[Languages]
Name: "chinesesimplified"; MessagesFile: "compiler:Languages\ChineseSimplified.isl"

[Types]                                   
Name: "program_only"; Description: "ֻ��װ����"
Name: "full"; Description: "������װ"
Name: "custom"; Description: "�Զ��尲װ"; Flags: iscustom

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";

[Components]
Name: "maincomp"; Description: "������"; Types: full program_only custom; Flags: fixed;
Name: "fileassoc"; Description: "�ļ�����"; Types: full;
{#ComponentAssoc("mp3")}         
{#ComponentAssoc("wav")}
{#ComponentAssoc("wma")}
{#ComponentAssoc("mid")}
{#ComponentAssoc("midi")}
{#ComponentAssoc("ogg")}
{#ComponentAssoc("aac")}
{#ComponentAssoc("flac")} 
{#ComponentAssoc("m4a")}

[Files]
Source: "..\miracleplayer_release\*"; DestDir: "{app}"; Components: "maincomp"; Flags: ignoreversion recursesubdirs createallsubdirs

[Registry]     
Root: HKA; Subkey: "Software\Classes\Applications\{#MyAppExeName}"; ValueType: none; ValueName: ""; ValueData: ""; Components: "maincomp"; Flags: uninsdeletekey
{#InstallAssoc("mp3")}       
{#InstallAssoc("wav")}
{#InstallAssoc("wma")}
{#InstallAssoc("mid")}
{#InstallAssoc("midi")}
{#InstallAssoc("ogg")}
{#InstallAssoc("aac")}
{#InstallAssoc("flac")} 
{#InstallAssoc("m4a")}
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}"; ValueType: string; ValueName: ""; ValueData: "{#MyAppAssocName}"; Components: "maincomp"; Flags: uninsdeletekey
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\{#MyAppExeName},0"; Components: "maincomp";
Root: HKA; Subkey: "Software\Classes\{#MyAppAssocKey}\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\{#MyAppExeName}"" ""%1"""; Components: "maincomp";  

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[UninstallRun]
Filename: "taskkill.exe"; Parameters: "/f /im {#MyAppExeName}"; Flags: waituntilterminated

[UninstallDelete]                  
Type: filesandordirs; Name: "{app}";

[Code]
procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
  case CurUninstallStep of
    usUninstall:
      begin
        if SuppressibleMsgBox('�Ƿ����û����ݣ�', mbConfirmation, MB_YESNO, IDYES) = IDNO then
        begin
          RegDeleteKeyIncludingSubkeys(HKCU, 'Software\MiraclePlayerGroup')
        end;
      end
  end;
end;
