rmdir /q/s bin_debug
rmdir /q/s bin_release
rmdir /q/s lib

rmdir /q/s x64

rmdir /q/s Kwatta\x64
rmdir /q/s Marlin\x64
rmdir /q/s ResultViewer\x64
rmdir /q/s StepEditor\x64
rmdir /q/s StyleFramework\x64
rmdir /q/s SuiteLibrary\x64
rmdir /q/s ValidateEditor\x64
rmdir /q/s TestEditor\x64
rmdir /q/s TestRunner\x64
rmdir /q/s Installer\x64

del Installer\Setup_Kwatta*.exe

del /s *.aps
del /s *.user

attrib -h  .vs
rmdir /q/s .vs
