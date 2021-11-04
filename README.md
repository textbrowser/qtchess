Android, FreeBSD, Linux, MacOS, OpenBSD, and Windows. Just about anywhere.

C++11 is required. Qt 5 LTS and Qt 6 LTS are supported.

Data authentication is provided by SHA-1 HMAC. Encryption is not necessary! 

Key = (Local Address xor Peer Address) || (Local Port xor Peer Port) || (Caissa)

Digest = HMAC(Data, HMAC(Key, "QtChess" || 0x01))

![play](https://github.com/textbrowser/qtchess/blob/master/Images/play.png)
