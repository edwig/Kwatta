rmdir /q/s bin_debug
rmdir /q/s bin_release
rmdir /q/s bin_debugUnicode
rmdir /q/s bin_releaseUnicode
rmdir /q/s lib
rmdir /q/s x64

call :RmSubDirs BaseLibrary
call :RmSubDirs Installer
call :RmSubDirs Kwatta
call :RmSubDirs Marlin
call :RmSubDirs QLLanguage
call :RmSubDirs ResultViewer
call :RmSubDirs SQLComponents
call :RmSubDirs StepEditor
call :RmSubDirs StyleFramework
call :RmSubDirs SuiteLibrary
call :RmSubDirs TestEditor
call :RmSubDirs TestRunner
call :RmSubDirs ValidateEditor

del Installer\Setup_Kwatta*.exe

del /s *.aps
del /s *.user

attrib -h  .vs
rmdir /q/s .vs

goto :end

:RmSubDirs

RmDir  /q /s %1\Debug
RmDir  /q /s %1\DebugUnicode
RmDir  /q /s %1\Release
RmDir  /q /s %1\ReleaseUnicode
RmDir  /q /s %1\x64
RmDir  /q /s %1\Win32
goto :end

:end