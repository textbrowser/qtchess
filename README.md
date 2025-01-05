Online chess program. Available on Android, FreeBSD, Linux, MacOS, OpenBSD,
and Windows. Just about anyware.

QtChess also interfaces with GNUChess!

C++11 is required. C++17 for Qt 6.

Debian PowerPC is Qt 5.5.1. Notice QT_SHA3_KECCAK_COMPAT in qtchess.pro.

Qt LTS is supported. Qt 4.8.x is obsolete.

Data authentication is provided by SHA3-512 HMAC. Encryption is not necessary!

Key = Caissa

Digest = HMAC(Data, HMAC(Key, "QtChess" || QtChess-Version || 0x01))

![play](https://github.com/textbrowser/qtchess/blob/master/Images/play.png)
