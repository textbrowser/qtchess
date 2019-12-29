#!/bin/sh

if [ -r ./QtChess ] && [ -x ./QtChess ]
then
    ./QtChess -style fusion "$@"
    exit $?
elif [ -r /usr/local/qtchess/QtChess ] && [ -x /usr/local/qtchess/QtChess ]
then
    cd /usr/local/qtchess && exec ./QtChess -style fusion "$@"
    exit $?
else
    exit 1
fi
