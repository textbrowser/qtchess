#!/usr/bin/env sh

# Alexis Megas.

set_qt_qpa_platformtheme()
{
    qt6=$(ldd "$1" | grep Qt6 2>/dev/null)

    if [ ! -z "$qt6" ]
    then
	if [ ! -z "$(which qt6ct)" ]
	then
	    echo "Exporting QT_QPA_PLATFORMTHEME as qt6ct."

	    export QT_QPA_PLATFORMTHEME=qt6ct
	fi
    fi
}

export QT_AUTO_SCREEN_SCALE_FACTOR=1

# Disable https://en.wikipedia.org/wiki/MIT-SHM.

export QT_X11_NO_MITSHM=1

if [ -r ./QtChess ] && [ -x ./QtChess ]
then
    echo "Launching a local QtChess."
    set_qt_qpa_platformtheme "./QtChess"
    ./QtChess "$@"
    exit $?
elif [ -r /opt/qtchess/QtChess ] && [ -x /opt/qtchess/QtChess ]
then
    echo "Launching an official QtChess."
    set_qt_qpa_platformtheme "/opt/qtchess/QtChess"
    /opt/qtchess/QtChess "$@"
    exit $?
else
    echo "Cannot locate QtChess."
    exit 1
fi
