Android, FreeBSD, Linux, MacOS, OpenBSD, and Windows. Just about anywhere.

C++11 is required. C++17 for Qt 6.

Qt 5.15 LTS, Qt 6.2 LTS, and Qt 6.5 LTS are supported.

Data authentication is provided by SHA-1 HMAC. Encryption is not necessary!

Key = (Local Address xor Peer Address) || (Local Port xor Peer Port) || (Caissa)

Digest = HMAC(Data, HMAC(Key, "QtChess" || 0x01))

![play](https://github.com/textbrowser/qtchess/blob/master/Images/play.png)
