; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "TopPIC"
#define MyAppVersion "0.9.1"
#define MyAppPublisher "Indiana University"
#define MyAppExeName "toppic.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{53F475EC-67DA-4459-8151-2CE395A7D751}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName=C:\{#MyAppName}
DisableDirPage=yes
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
LicenseFile=C:\Users\qkou\Desktop\toppic-win\LICENSE.txt
OutputDir=C:\Users\qkou\Desktop\toppic-win
OutputBaseFilename=toppic-setup-0.9.1
SetupIconFile=C:\Users\qkou\Desktop\toppic-win\toppic.ico
Compression=lzma
SolidCompression=yes
ChangesEnvironment=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "C:\Users\qkou\Desktop\toppic-win\toppic.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\qkou\Desktop\proteomics_cpp\libgcc_s_seh-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\qkou\Desktop\proteomics_cpp\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\qkou\Desktop\proteomics_cpp\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\qkou\Desktop\toppic-win\conf\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"

[Registry]
Root: HKLM; Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; ValueType: expandsz; ValueName: "Path"; ValueData: "{olddata};{app}"