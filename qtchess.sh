#!/usr/bin/env sh
# Alexis Megas.

# Enable if necessary.

export QT_AUTO_SCREEN_SCALE_FACTOR=1

# Disable https://en.wikipedia.org/wiki/MIT-SHM.

export QT_X11_NO_MITSHM=1

if [ -r ./QtChess ] && [ -x ./QtChess ]
then
    echo "Launching a local QtChess."
    ./QtChess "$@"
    exit $?
elif [ -r /opt/qtchess/QtChess ] && [ -x /opt/qtchess/QtChess ]
then
    echo "Launching an official QtChess."
    cd /opt/qtchess && exec ./QtChess "$@"
    exit $?
else
    echo "Cannot locate QtChess."
    exit 1
fi
