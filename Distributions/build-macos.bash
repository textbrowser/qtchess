#!/usr/bin/env bash
# Alexis Megas.
# Qt LTS only please!

# Please execute this script in an Apple terminal because
# Apple may prompt for a certificate password (codesign).

if [ ! -e qtchess.pro ]
then
    echo "Please issue $0 from the primary directory."
    exit 1
fi

if [ ! -z "${SSH_TTY}" ]
then
    echo "SSH session detected. " \
	 "MacOS codesign password prompt may be invisible."
fi

make distclean 1>/dev/null 2>/dev/null

declare -a qmakes=("$HOME/Qt/6.12.0/macos/bin/qmake"
		   "$HOME/Qt/6.8.3/macos/bin/qmake")
qmake=""

for i in "${qmakes[@]}"
do
    qmake="$(echo $i)"

    if [ -x "$qmake" ]
    then
	break
    fi
done

if [ -x "$qmake" ]
then
    echo "Found $qmake. Creating Makefile."
    $qmake -o Makefile qtchess.pro 1>/dev/null 2>/dev/null
else
    echo "Cannot locate qmake. Please install the official Qt."
    exit 1
fi

echo "Making QtChess."
make -j $(sysctl -n hw.ncpu) 1>/dev/null 2>/dev/null
make install 1>/dev/null 2>/dev/null
echo "Signing ./QtChess.d/QtChess.app."
codesign --deep --force -s "textbrowser" ./QtChess.d/QtChess.app \
	 1>/dev/null 2>/dev/null

if [ ! $? -eq 0 ]
then
    echo "Signing error. Bye!"
    exit 1
fi

echo "Building QtChess.d.dmg."
make dmg 1>/dev/null 2>/dev/null

if [ ! -r QtChess.d.dmg ]
then
    echo "QtChess.d.dmg is not a readable file."
    exit 1
fi

mv QtChess.d.dmg QtChess-2026.11.12_Universal.dmg
make distclean 1>/dev/null 2>/dev/null
rm -fr ./QtChess.d
