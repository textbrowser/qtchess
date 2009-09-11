#!/bin/sh

ostype="`uname -s`"

if [ $ostype = "Darwin" ]
then
    open /Applications/QtChess.d/QtChess.app &
else
    cd /usr/local/qtchess
    ./QtChess &
fi

exit 0
