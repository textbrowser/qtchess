#!/usr/bin/env sh
# Alexis Megas.

if [ ! -x /usr/bin/dpkg-deb ]; then
    echo "Please install dpkg-deb."
    exit
fi

if [ ! -x /usr/bin/fakeroot ]; then
    echo "Please install fakeroot."
    exit 1
fi

if [ ! -r qtchess.pro ]; then
    echo "Please execute $0 from the primary directory."
    exit 1
fi

# Prepare local files.

mkdir -p ./usr/local/qtchess
qmake -o Makefile qtchess.pro && make -j $(nproc)
cp ./QtChess ./usr/local/qtchess/.
cp ./qtchess.sh ./usr/local/qtchess/.
find ./usr/local/qtchess -type f -exec chmod g+w {} \;

# Prepare the Debian file.

mkdir -p qtchess-debian/usr/local
cp -pr ./PiOS64 qtchess-debian/DEBIAN
cp -r ./usr/local/qtchess qtchess-debian/usr/local/.
fakeroot dpkg-deb --build qtchess-debian QtChess-2024.01.10_arm64.deb
make distclean
rm -fr ./qtchess-debian
rm -fr ./usr
