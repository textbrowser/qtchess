# Define installer name.
Name "QtChess Installer"
outFile "QtChess-Installer.exe"

# Install directory.
installDir C:\QtChess

DirText "Please choose an installation directory for QtChess."

# Default section start.
section

# Define output path.
setOutPath $INSTDIR

# Specify files to go in output path.
file /r .\release\Images
file /r .\release\plugins
file .\release\qt.conf
file .\release\*.dll
file .\release\QtChess.exe

# Define uninstaller name.
writeUninstaller $INSTDIR\QtChess-Uninstaller.exe

# Default section end.
sectionEnd

# Create a section to define what the uninstaller does.
# The section will always be named "Uninstall".
section "Uninstall"

# Always delete uninstaller first.
RMDir /r $INSTDIR

sectionEnd
