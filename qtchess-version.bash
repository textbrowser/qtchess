#!/usr/bin/env bash

echo "The command sed may fail on MacOS."

VERSION=$1

if [ -z "$VERSION" ]; then
    echo "Please specify the version: $0 <VERSION>."
    exit 1
fi

for file in */control; do
    sed -i "s/Version: .*/Version: $VERSION/" $file
done

for file in Distributions/build*; do
    sed -i \
    "s/QtChess-[[:digit:]]\+\.[[:digit:]]\+\.[[:digit:]]\+/QtChess-$VERSION/" \
    $file
done

FILE="Source/qtchess.h"

sed -i 's/\(QTCHESS_VERSION "\)[0-9]\+\(\.[0-9]\+\)*"/\1'"$VERSION"'"/' \
    $FILE

FILE="Android/AndroidManifest.xml"

sed -i \
    's/\(android:versionCode="\)[0-9]\+\([0-9]\+\)*"/\1'"${VERSION//./}"'"/' \
    $FILE
sed -i \
    's/\(android:versionName="\)[0-9]\+\(\.[0-9]\+\)*"/\1'"$VERSION"'"/' \
    $FILE

