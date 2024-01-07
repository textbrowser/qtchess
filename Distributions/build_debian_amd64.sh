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

mkdir -p ./opt/qtchess
qmake -o Makefile qtchess.pro && make -j $(nproc)
cp ./QtChess ./opt/qtchess/.
cp ./qtchess.sh ./opt/qtchess/.
find ./opt/qtchess -type f -exec chmod g+w {} \;

# Prepare the Debian file.

mkdir -p qtchess-debian/opt
cp -pr ./DEBIAN qtchess-debian/.
cp -r ./opt/qtchess qtchess-debian/opt/.
fakeroot dpkg-deb --build qtchess-debian QtChess-2024.01.10_amd64.deb
make distclean
rm -fr ./opt
rm -fr ./qtchess-debian
