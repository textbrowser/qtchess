#!/bin/sh

if [ -x /usr/local/qtchess/QtChess ]
then
    cd /usr/local/qtchess
    ./QtChess $* &
else
    exit 1
fi

exit 0
