Android, FreeBSD, Linux, MacOS, OpenBSD, and Windows. Just about anyware.

C++11 is required. C++17 for Qt 6.

Qt LTS is supported. Qt 4.8.x is obsolete.

Data authentication is provided by SHA3-512 HMAC. Encryption is not necessary!

Key = Caissa

Digest = HMAC(Data, HMAC(Key, "QtChess" || QtChess-Version || 0x01))

![play](https://github.com/textbrowser/qtchess/blob/master/Images/play.png)
