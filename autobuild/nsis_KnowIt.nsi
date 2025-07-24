; KnowIt Installation Script (ASCII-only, no plugins required)
Name "KnowIt"

!ifndef OUTDIR
  !define OUTDIR "D:\work\output\KnowIt"
!endif

!ifndef TIMESTAMP
  !define TIMESTAMP "unknown"
!endif

OutFile "${OUTDIR}\KnowIt_Setup_${TIMESTAMP}.exe"
InstallDir "$PROGRAMFILES\KnowIt"
RequestExecutionLevel admin

!include MUI2.nsh

!define MUI_ABORTWARNING
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
Page custom OptionsPage OptionsPageLeave
!insertmacro MUI_PAGE_INSTFILES
!define MUI_PAGE_CUSTOMFUNCTION_PRE LaunchApp
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

Var RemoveConfig
Var RemoveLog
Var LaunchProgram

!define REGKEY "Software\KnowIt"
!define REGVALUE_INSTALLDIR "InstallDir"

Function .onInit
  ; Initialize plugins directory for temporary files
  InitPluginsDir

  ; Extract kill_KnowIt.bat from the source directory
  File /nonfatal /oname=$PLUGINSDIR\kill_KnowIt.bat "D:\work\output\KnowIt\latest\kill_KnowIt.bat"

  ; Run the batch file to terminate KnowIt.exe
  nsExec::ExecToStack '"$PLUGINSDIR\kill_KnowIt.bat"'
  Pop $0 ; Return value
  Pop $1 ; Output (if any)

  ; Check if the batch file failed (non-zero return code)
  StrCmp $0 0 +3
    MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION "Cannot terminate KnowIt.exe. Please close it manually and retry, or cancel to abort." IDRETRY retry
    Abort
  retry:
    nsExec::ExecToStack '"$PLUGINSDIR\kill_KnowIt.bat"'
    Pop $0
    Pop $1
    StrCmp $0 0 +2
      MessageBox MB_OK|MB_ICONEXCLAMATION "Failed to terminate KnowIt.exe. Continuing installation anyway."

  ; Read the previous installation path from the registry
  ReadRegStr $INSTDIR HKLM "${REGKEY}" "${REGVALUE_INSTALLDIR}"
FunctionEnd

Function OptionsPage
  !insertmacro MUI_HEADER_TEXT "Installation Options" "Select which folders to keep and whether to launch the program"
  nsDialogs::Create 1018
  Pop $0

  ${NSD_CreateCheckbox} 0 10u 100% 10u "Keep config directory"
  Pop $RemoveConfig
  ${NSD_SetState} $RemoveConfig ${BST_CHECKED}

  ${NSD_CreateCheckbox} 0 30u 100% 10u "Keep log directory"
  Pop $RemoveLog
  ${NSD_SetState} $RemoveLog ${BST_CHECKED}

  ${NSD_CreateCheckbox} 0 50u 100% 10u "Launch KnowIt after installation"
  Pop $LaunchProgram
  ${NSD_SetState} $LaunchProgram ${BST_CHECKED}

  nsDialogs::Show
FunctionEnd

Function OptionsPageLeave
  ${NSD_GetState} $RemoveConfig $RemoveConfig
  ${NSD_GetState} $RemoveLog $RemoveLog
  ${NSD_GetState} $LaunchProgram $LaunchProgram
FunctionEnd

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite on

  ; Install all files (except config, log, and kill_KnowIt.bat)
  File /r /x "config" /x "log" /x "kill_KnowIt.bat" "D:\work\output\KnowIt\latest\*.*"

  ; Install config and log directories based on user selection
  ${If} $RemoveConfig == ${BST_CHECKED}
    File /nonfatal /r "D:\work\output\KnowIt\latest\config"
  ${EndIf}

  ${If} $RemoveLog == ${BST_CHECKED}
    File /nonfatal /r "D:\work\output\KnowIt\latest\log"
  ${EndIf}

  ; Create uninstaller
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${REGKEY}" "${REGVALUE_INSTALLDIR}" "$INSTDIR"
SectionEnd

Section "Uninstall"
  RMDir /r "$INSTDIR"
  DeleteRegKey HKLM "${REGKEY}"
SectionEnd

Function LaunchApp
  ${If} $LaunchProgram == ${BST_CHECKED}
    ExecShell "" "$INSTDIR\KnowIt.exe"
  ${EndIf}
FunctionEnd