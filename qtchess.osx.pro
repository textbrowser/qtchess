purge.commands = rm -f *~ && rm -f */*~

CONFIG		+= app_bundle qt release warn_on
ICON            = Images/chess.icns
LANGUAGE        = C++
QT		+= network opengl
TEMPLATE        = app

QMAKE_CLEAN	+= QtChess
QMAKE_CXXFLAGS  += -Wall \
                   -Wcast-align \
                   -Wcast-qual \
                   -Wconversion \
                   -Werror \
                   -Wextra \
                   -Wformat=2 \
                   -Woverloaded-virtual \
                   -Wpointer-arith \
                   -Wsign-conversion \
                   -Wstack-protector \
                   -Wstrict-overflow=5 \
                   -fPIE \
                   -fstack-protector-all \
                   -fwrapv \
                   -mtune=generic \
                   -pedantic
QMAKE_EXTRA_TARGETS = purge

RESOURCES       = Images/images.qrc

FORMS		= UI/helpDialog.ui \
                  UI/mainwindow.ui \
                  UI/promotionDialog.ui \
                  UI/setupDialog.ui

HEADERS		= Source/qtchess.h \
                  Source/qtchess_comm.h \
                  Source/qtchess_defs.h \
                  Source/qtchess_gui.h \
                  Source/qtchess_gui_opengl.h \
                  Source/qtchess_validate.h

SOURCES		= Source/qtchess.cc \
                  Source/qtchess_comm.cc \
                  Source/qtchess_gui.cc \
                  Source/qtchess_gui_opengl.cc \
                  Source/qtchess_main.cc \
                  Source/qtchess_validate.cc

greaterThan(QT_MAJOR_VERSION, 4) {
LIBS            += -framework Cocoa
OBJECTIVE_HEADERS += Source/CocoaInitializer.h
OBJECTIVE_SOURCES += Source/CocoaInitializer.mm
}

PROJECTNAME	= QtChess
TARGET		= QtChess

macdeployqt.path  = QtChess.app
macdeployqt.extra = $$[QT_INSTALL_BINS]/macdeployqt ./QtChess.app

INSTALLS	= macdeployqt
