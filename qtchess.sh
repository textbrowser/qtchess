#!/bin/sh

# Enable if necessary.

export QT_AUTO_SCREEN_SCALE_FACTOR=1

# Disable https://en.wikipedia.org/wiki/MIT-SHM.

export QT_X11_NO_MITSHM=1

if [ -r ./QtChess ] && [ -x ./QtChess ]
then
    ./QtChess "$@"
    exit $?
elif [ -r /usr/local/qtchess/QtChess ] && [ -x /usr/local/qtchess/QtChess ]
then
    cd /usr/local/qtchess && exec ./QtChess "$@"
    exit $?
else
    exit 1
fi
