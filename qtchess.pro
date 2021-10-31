include(qtchess-source.pro)

win {
purge.commands = del *~ && del */*~
} else {
purge.commands = rm -f *~ && rm -f */*~
}

CONFIG		+= qt release warn_on
DEFINES		+= QTCHESS_DEBUG QT_DEPRECATED_WARNINGS

mac {
ICON            = Images/chess.icns
} else:win {
ICON            = Images/chess.png
}

LANGUAGE        = C++
QT		+= gui network widgets

win {
RC_FILE		= qtchess.win.rc
}

TEMPLATE        = app

QMAKE_CLEAN	+= QtChess

linux {
QMAKE_CXXFLAGS  += -Wall \
                   -Wcast-align \
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
                   -Wstrict-overflow=5 \
                   -Wundef \
                   -Wzero-as-null-pointer-constant \
                   -fPIE \
                   -fstack-protector-all \
                   -fwrapv \
                   -pedantic \
                   -pie \
                   -std=c++11
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
} else:win {
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

unix {
QMAKE_EXTRA_TARGETS = purge
}

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
}
