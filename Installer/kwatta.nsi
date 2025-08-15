;-------------------------------------------------------
;
; Project:      Kwatta API Testtool
; Auteurs:      Edo Huisman
;
; Copyright (c) 2021-2025 ir. W.E. Huisman
; All rights reserved
;
; Last change:       15-08-2025
; Versionnumber:     1.3.5
;-------------------------------------------------------
 !define PRODUCT_NAME                         "Kwatta"
 !define PRODUCT_VERSION                      "1.3.5"
 !define PRODUCT_BUILDNUMBER                  "290"
 !define PRODUCT_PUBLISHER                    "EDO"
 !define PRODUCT_WEB_SITE                     "https://github.com/edwig/kwatta"
 !define PRODUCT_DIR_REGKEY                   "Software\Microsoft\Windows\CurrentVersion\App Paths\${PRODUCT_NAME}"
 !define PRODUCT_UNINST_KEY                   "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
 !define PRODUCT_UNINST_ROOT_KEY              "HKLM"
 ;!define MUI_HEADERIMAGE
 ;!define MUI_HEADERIMAGE_BITMAP              "..\Ateles_paniscus.jpg"
 !define MUI_DIRECTORYPAGE_VERIFYONLEAVE
 
 ; Path for the input files
 !define InputDir             "..\Bin_Release\"
 !define InputDirUnicode      "..\Bin_ReleaseUnicode\"
 !define RootDir              "..\"

;--------------------------------------------------------------------------------------------------------
 var currentVersion

 ;--------------------------------------------------------------------------------------------------------
 !include "MUI2.nsh"
 !include LogicLib.nsh
 !include "FileFunc.nsh"
 !include x64.nsh
 !include WinMessages.nsh
 !include FileAssoc.nsh
 ;--------------------------------------------------------------------------------------------------------
 ;macro's
  
 !macro CopyFile path
  ${If} ${path} != ""
   DetailPrint "Copy from [${path}] to [$INSTDIR]"
   CopyFiles /FILESONLY "${path}" "$INSTDIR"
  ${EndIf}

 !macroend  

 !macro _CreateShortcutInStartMenuPrograms targetName program targetExtension
   SetShellVarContext all
   CreateShortCut "$INSTDIR\${targetName}.lnk" "$INSTDIR\${program}.${targetExtension}"
   CreateShortCut "$INSTDIR\${targetName}_Unicode.lnk" "$INSTDIR\Unicode\${program}.${targetExtension}"
   CopyFiles /FILESONLY "$INSTDIR\${targetName}.lnk"         "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}_${PRODUCT_VERSION}\${targetName}.lnk"
   CopyFiles /FILESONLY "$INSTDIR\${targetName}_Unicode.lnk" "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}_${PRODUCT_VERSION}\${targetName}_Unicode.lnk"
   Delete "$INSTDIR\${targetName}.lnk"
   Delete "$INSTDIR\${targetName}_Unicode.lnk"
 !macroend
 
 !define CreateShortcutInStartMenuPrograms "!insertmacro _CreateShortcutInStartMenuPrograms"

 ;--------------------------------------------------------------------------------------------------------
 
 ; Compression saves about 40% diskspace
 SetCompressor LZMA
 SetCompressorDictSize 8
 ;--------------------------------------------------------------------------------------------------------
 SetPluginUnload alwaysoff
 BrandingText /TRIMRIGHT "Edo Huisman"
 InstallColors 000000 FFFFFF
 XPStyle on
 RequestExecutionLevel admin
;--------------------------------------------------------------------------------------------------------
 OutFile "Setup_${PRODUCT_NAME}_${PRODUCT_VERSION}.exe"
;--------------------------------------------------------------------------------------------------------
; title of the setup
 Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
;--------------------------------------------------------------------------------------------------------
 ; Set in the standard installation folder
 ; Beware: The "\" at the end must be preserved to prevent 'strange' browser behaviour
 InstallDir  "$PROGRAMFILES\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}\"

;--------------------------------------------------------------------------------------------------------
 ; MUI Settings
 !define MUI_ICON   "Kwatta.ico"
 !define MUI_UNICON "Kwatta.ico"
 !define MUI_BGCOLOR F0F0F0
 !define MUI_HEADER_TRANSPARENT_TEXT
 !define MUI_WELCOMEFINISHPAGE_BITMAP "jungledevil.bmp"
 !define MUI_LICENSEPAGE_BGCOLOR /grey
 !define MUI_COMPONENTSPAGE_NODESC
 !define MUI_COMPONENTSPAGE_TEXT_COMPLIST "To be installed"
 !define MUI_FINISHPAGE_NOAUTOCLOSE
 !define MUI_FINISHPAGE_RUN "$INSTDIR\Kwatta.exe"
 !define MUI_UNWELCOMEFINISHPAGE_BITMAP "jungledevil.bmp"
 !define MUI_UNFINISHPAGE_NOAUTOCLOSE
 !define MUI_ABORTWARNING
 !define MUI_ABORTWARNING_TEXT "Do you wish to abort the installation of ${PRODUCT_NAME} ?"
 !define MUI_ABORTWARNING_CANCEL_DEFAULT
 !define MUI_UNABORTWARNING
 !define MUI_UNABORTWARNING_TEXT "Do you want to abort the de-installation of ${PRODUCT_NAME} ?"
 !define MUI_UNABORTWARNING_CANCEL_DEFAULT

 
;--------------------------------------------------------------------------------------------------------
 ; Language Selection Dialog Settings
 !define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
 !define MUI_LANGDLL_REGISTRY_KEY  "${PRODUCT_UNINST_KEY}"
 !define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"
 !define MUI_LANGDLL_WINDOWTITLE "Language"
 !define MUI_LANGDLL_INFO "Select a language for the ${PRODUCT_NAME} installer"
;--------------------------------------------------------------------------------------------------------

 ; Welcome page
 !insertmacro MUI_PAGE_WELCOME
;--------------------------------------------------------------------------------------------------------
; Directory page
 !insertmacro MUI_PAGE_LICENSE "License.txt"
 !insertmacro MUI_PAGE_COMPONENTS
 !insertmacro MUI_PAGE_DIRECTORY
 !insertmacro MUI_PAGE_INSTFILES
;--------------------------------------------------------------------------------------------------------
 !insertmacro MUI_PAGE_FINISH
;--------------------------------------------------------------------------------------------------------
; Uninstaller pages
 !insertmacro MUI_UNPAGE_INSTFILES
;--------------------------------------------------------------------------------------------------------
; Language files
 !insertmacro MUI_LANGUAGE "English"
;--------------------------------------------------------------------------------------------------------
 ShowInstDetails   show
 ShowUnInstDetails show
;--------------------------------------------------------------------------------------------------------


;--------------------------------------------------------------------------------------------------------
Section "The Program" prog_always

 ;CreateDirectory "$INSTDIR\Ansi"
  CreateDirectory "$INSTDIR"
 
 ; Explicit overwrite of files
 SetOverwrite on
 
 ;SetOutPath  "$INSTDIR\Ansi"
 ;DetailPrint "Output directory set to: ${INSTDIR}\Ansi"
 ;DetailPrint "Copying the files..."
 ;File "${RootDir}TheName.md"
 ;File "${RootDir}License.md"
 ;File "${InputDir}Kwatta.exe"
 ;File "${InputDir}TestEditor.exe"
 ;File "${InputDir}StepEditor.exe"
 ;File "${InputDir}ValidateEditor.exe"
 ;File "${InputDir}ResultViewer.exe"
 ;File "${InputDir}TestRunner.exe"
 SetOutPath "$INSTDIR"
 DetailPrint "Output directory set to: $INSTDIR"
 DetailPrint "Copying the files..."
 File "${RootDir}TheName.md"
 File "${RootDir}License.md"
 File "${InputDirUnicode}Kwatta.exe"
 File "${InputDirUnicode}TestEditor.exe"
 File "${InputDirUnicode}StepEditor.exe"
 File "${InputDirUnicode}ValidateEditor.exe"
 File "${InputDirUnicode}ResultViewer.exe"
 File "${InputDirUnicode}TestRunner.exe"
 SetOutPath  "$INSTDIR"
SectionEnd

Section "Create startmenu" Create_Startmenu

 DetailPrint "Creating shortcuts in the startmenu"
 SetOutPath "$INSTDIR"
 CreateDirectory "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}_${PRODUCT_VERSION}"

 ${CreateShortcutInStartMenuPrograms} Kwatta  Kwatta exe
SectionEnd

Section "Create desktop icons" Desktop_icons

 DetailPrint "Creating of the desktop shortcuts"
 SetOutPath "$INSTDIR"
 ;CreateShortCut "$DESKTOP\Kwatta_ANSI.lnk" "$INSTDIR\Ansi\Kwatta.exe"
  CreateShortCut "$DESKTOP\Kwatta.lnk"      "$INSTDIR\Kwatta.exe"
SectionEnd


;--------------------------------------------------------------------------------------------------------
; To do at the start
; Function must come after all the sections, because all the variables are now declared
;
Function .onInit
 ClearErrors

 ; Always install the programs
 IntOp $0 ${SF_RO} | ${SF_SELECTED}
 SectionSetFlags ${prog_always} $0

;Check if there is a newer version of the product
 Readregstr $currentVersion "${PRODUCT_UNINST_ROOT_KEY}" "${PRODUCT_UNINST_KEY}" "BuildNumber"
;MessageBox MB_OK "The current version is $currentVersion"
;MessageBox MB_OK "The install version is ${PRODUCT_BUILDNUMBER}"
 IntCmp $currentVersion ${PRODUCT_BUILDNUMBER} versionTheSame SetupVersionNewer CurrentVersionNewer

 versionTheSame:
  Messagebox MB_YESNO "This version of ${PRODUCT_NAME} is already installed. Do you want to re-install it? " /SD IDYES IDNO doNotRepair
  goto SetupVersionNewer

 doNotRepair:
  abort

 CurrentVersionNewer:
  Messagebox MB_YESNO "There is already a newer version of ${PRODUCT_NAME} installed on your system. \
             Do you want to overwrite it with the current (lower) version of the product?" \
            /SD IDNO IDYES doInstallAfterAll

  abort
 doInstallAfterAll:
 Delete "$INSTDIR\Uninstall ${PRODUCT_NAME} $currentVersion.exe"

 SetupVersionNewer:
;  InitPluginsDir
;  File /oname=$PLUGINSDIR\edo.png "edo.png"
;  IfSilent +2
;  advsplash::show 1000 1000 500 0xFFFFFF $PLUGINSDIR\edo

; !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd


;--------------------------------------------------------------------------------------------------------
; If installation succeeded, create the uninstaller and the product info
Section -Post
 WriteUninstaller "$INSTDIR\Uninstall_${PRODUCT_NAME}_${PRODUCT_VERSION}.exe"
 
 WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName"     "$(^Name)"
 WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\Uninstall_${PRODUCT_NAME}_${PRODUCT_VERSION}.exe"
 WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon"     "$INSTDIR\Uninstall_${PRODUCT_NAME}_${PRODUCT_VERSION}.exe"
 WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion"  "${PRODUCT_VERSION}.${PRODUCT_BUILDNUMBER}"
 WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout"    "${PRODUCT_WEB_SITE}" 
 WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher"       "${PRODUCT_PUBLISHER}"
 WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "BuildNumber"     "${PRODUCT_BUILDNUMBER}"

 !insertmacro APP_ASSOCIATE "xtest" "Kwatta.XTest" "Kwatta API Test suite"  "$INSTDIR\Kwatta.exe,0" "Open with Kwatta" "$INSTDIR\Kwatta.exe $\"%1$\""
 !insertmacro APP_ASSOCIATE_ADDVERB "xtest" "open" "Open Kwatta XTest suite"  "$INSTDIR\Kwatta.exe $\"%1$\""


 ${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
 IntFmt $0 "0x%08X" $0
 WriteRegDWORD ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "EstimatedSize" "$0"
 
 ; Last message
 MessageBox MB_OK "The installation of ${PRODUCT_NAME} ${PRODUCT_VERSION} is ready." /SD IDOK
SectionEnd

;--------------------------------------------------------------------------------------------------------
Function un.onUninstSuccess
 HideWindow
 MessageBox MB_ICONINFORMATION|MB_OK "${PRODUCT_NAME} version: ${PRODUCT_VERSION} was successfully removed from your computer." /SD IDOK 
FunctionEnd

;--------------------------------------------------------------------------------------------------------
Function un.onInit
 MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure that you want to remove ${PRODUCT_NAME} version: ${PRODUCT_VERSION} and all it's components from your computer?" /SD IDYES IDYES +2
 Abort
FunctionEnd

;--------------------------------------------------------------------------------------------------------
Section Uninstall
 SetAutoClose true
 SetShellVarContext all
 DetailPrint "De-installation and de-registration of  ${PRODUCT_NAME} version: ${PRODUCT_VERSION} buildnumber ${PRODUCT_BUILDNUMBER}"

 DetailPrint "Removing sub-directories from: $INSTDIR"
 RmDir /r /REBOOTOK "$INSTDIR\Data"

 DetailPrint "Removing the files from: $INSTDIR"
 RmDir /r "$INSTDIR"

 ; Remove directory in the start menu
 DetailPrint "Remove links from the startmenu"
 Delete "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}_${PRODUCT_VERSION}\Kwatta_ANSI.lnk"
 Delete "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}_${PRODUCT_VERSION}\Kwatta.lnk"
 RmDir /r /REBOOTOK "$SMPROGRAMS\${PRODUCT_PUBLISHER}\${PRODUCT_NAME}_${PRODUCT_VERSION}"

 DetailPrint "Removing the links from the desktop"
 Delete "$DESKTOP\Kwatta_ANSI.lnk"
 Delete "$DESKTOP\Kwatta.lnk"
 
 ;De-Registring of the product.
 DetailPrint "De-registration of ${PRODUCT_NAME}"
 Delete "$INSTDIR\Uninstall ${PRODUCT_NAME} $currentVersion.exe"
 DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"

 !insertmacro APP_UNASSOCIATE "xtest" "Kwatta.XTest"
 !insertmacro APP_ASSOCIATE_REMOVEVERB "Kwatta.XTest" "open"


 DetailPrint "End of de-registration of ${PRODUCT_NAME}"
 DetailPrint "End uninstaller ${PRODUCT_NAME} ${PRODUCT_VERSION}."
SectionEnd
;--------------------------------------------------------------------------------------------------------
; End of the script
