include(qtchess-source.pro)

win32 {
purge.commands = del *~ && del */*~
} else {
purge.commands = rm -f *~ && rm -f */*~
}

CONFIG		+= qt release warn_on
DEFINES		+= QT_DEPRECATED_WARNINGS

mac {
ICON            = Images/chess.icns
} else:win32 {
ICON            = Images/chess.png
}

LANGUAGE        = C++
QT		+= gui network widgets

win32 {
RC_FILE		= qtchess.win.rc
}

TEMPLATE        = app

QMAKE_CLEAN	+= QtChess

android {
QMAKE_CXXFLAGS  += -Wall \
                   -Wcast-align \
                   -Wcast-qual \
                   -Werror \
                   -Werror=format-security \
                   -Wextra \
                   -Wfloat-equal \
                   -Wformat=2 \
                   -Woverloaded-virtual \
                   -Wpointer-arith \
                   -Wstack-protector \
                   -Wstrict-overflow=5 \
                   -Wundef \
                   -Wzero-as-null-pointer-constant \
                   -fPIE \
                   -fstack-protector-all \
                   -fwrapv \
                   -pedantic \
                   -std=c++11
} else:freebsd-* {
QMAKE_CXXFLAGS  += -Wall \
                   -Wcast-align \
                   -Wcast-qual \
                   -Wdouble-promotion \
                   -Werror \
                   -Werror=format-security \
                   -Wextra \
                   -Wfloat-equal \
                   -Wformat=2 \
                   -Wno-deprecated-copy \
                   -Woverloaded-virtual \
                   -Wpointer-arith \
                   -Wstack-protector \
                   -Wstrict-overflow=5 \
                   -Wundef \
                   -Wzero-as-null-pointer-constant \
                   -fPIE \
                   -fstack-protector-all \
                   -fwrapv \
                   -pedantic \
                   -std=c++11
} else:linux {
greaterThan(QT_MAJOR_VERSION, 4):greaterThan(QT_MINOR_VERSION, 5) {
QMAKE_CXXFLAGS  += -Wall \
                   -Wcast-qual \
                   -Wdouble-promotion \
                   -Werror \
                   -Werror=format-security \
                   -Wextra \
                   -Wfloat-equal \
                   -Wformat=2 \
                   -Wformat-overflow=2 \
                   -Wno-deprecated-copy \
                   -Woverloaded-virtual \
                   -Wpointer-arith \
                   -Wstack-protector \
                   -Wstrict-overflow=1 \
                   -Wundef \
                   -Wzero-as-null-pointer-constant \
                   -fPIE \
                   -fstack-protector-all \
                   -fwrapv \
                   -pedantic \
                   -pie \
                   -std=c++11
} else {
                   QMAKE_CXXFLAGS  += -Wall \
                   -Wcast-qual \
                   -Wdouble-promotion \
                   -Werror \
                   -Werror=format-security \
                   -Wextra \
                   -Wfloat-equal \
                   -Wformat=2 \
                   -Wno-deprecated-copy \
                   -Woverloaded-virtual \
                   -Wpointer-arith \
                   -Wstack-protector \
                   -Wstrict-overflow=1 \
                   -Wundef \
                   -fPIE \
                   -fstack-protector-all \
                   -fwrapv \
                   -pedantic \
                   -pie \
                   -std=c++11
}
} else:mac {
QMAKE_CXXFLAGS  += -Wall \
                   -Wcast-align \
                   -Wcast-qual \
                   -Werror \
                   -Wextra \
                   -Wformat=2 \
                   -Woverloaded-virtual \
                   -Wpointer-arith \
                   -Wstack-protector \
                   -Wstrict-overflow=5 \
                   -fPIE \
                   -fstack-protector-all \
                   -fwrapv \
                   -mtune=generic \
                   -pedantic \
                   -std=c++11
} else:win32 {
QMAKE_CXXFLAGS  += -Wall \
                   -Wcast-align \
                   -Wcast-qual \
                   -Wextra \
                   -Wformat=2 \
                   -Woverloaded-virtual \
                   -Wpointer-arith \
                   -Wstrict-overflow=5 \
                   -fwrapv \
                   -pedantic \
                   -pie \
                   -std=c++11
}

QMAKE_DISTCLEAN += .qmake.cache .qmake.stash
QMAKE_EXTRA_TARGETS = purge

PROJECTNAME	= QtChess
TARGET		= QtChess

android {
DISTFILES += android/AndroidManifest.xml \
             android/build.gradle \
             android/gradle.properties \
             android/gradle/wrapper/gradle-wrapper.jar \
             android/gradle/wrapper/gradle-wrapper.properties \
             android/gradlew \
             android/gradlew.bat \
             android/res/values/libs.xml

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}
} else:mac {
macdeployqt.path  = QtChess.app
macdeployqt.extra = $$[QT_INSTALL_BINS]/macdeployqt ./QtChess.app

INSTALLS	= macdeployqt
} else:win32 {
plugins1.files = $$[QT_INSTALL_PLUGINS]\\*
plugins1.path = release\\plugins\\.
plugins2.files = $$[QT_INSTALL_PLUGINS]\\gamepads\\xinputgamepad.dll
plugins2.path = release\\plugins\\gamepads\\.
plugins3.files = $$[QT_INSTALL_PLUGINS]\\platforms\\qdirect2d.dll
plugins3.path = release\\plugins\\platforms\\.
plugins4.files = $$[QT_INSTALL_PLUGINS]\\renderplugins\\scene2d.dll
plugins4.path = release\\plugins\\renderplugins\\.
pluginspurge.extra = del /Q /S *d.dll
pluginspurge.path = release\\plugins\\.
qt.files = Qt\\qt.conf
qt.path = release\\.
qtlibraries.files = $$[QT_INSTALL_BINS]\\Qt5Core.dll \
                    $$[QT_INSTALL_BINS]\\Qt5Gui.dll \
                    $$[QT_INSTALL_BINS]\\Qt5Network.dll \
                    $$[QT_INSTALL_BINS]\\Qt5Widgets.dll \
                    $$[QT_INSTALL_BINS]\\libgcc_s_dw2-1.dll \
                    $$[QT_INSTALL_BINS]\\libstdc++-6.dll \
                    $$[QT_INSTALL_BINS]\\libwinpthread-1.dll
qtlibraries.path = release\\.

INSTALLS = plugins1 \
           pluginspurge \
           plugins2 \
           plugins3 \
           plugins4 \
           qt \
           qtlibraries
}
