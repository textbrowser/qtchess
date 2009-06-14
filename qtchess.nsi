# Define installer name.
Name "QtChess Installer"
outFile "QtChess-Installer-3.12.exe"

# Install directory.
installDir C:\QtChess-3.12

DirText "Please choose an installation directory for QtChess."

# Default section start.
section

# Define output path.
setOutPath $INSTDIR

# Specify files to go in output path.
file .\release\*.dll
file .\release\QtChess.exe

# Define uninstaller name.
writeUninstaller $INSTDIR\QtChess-Uninstaller-3.12.exe

# Default section end.
sectionEnd

# Create a section to define what the uninstaller does.
# The section will always be named "Uninstall".
section "Uninstall"

# Always delete uninstaller first.
RMDir /r $INSTDIR

sectionEnd
