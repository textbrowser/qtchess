cache()
purge.commands = rm -f *~ && rm -f */*~

CONFIG		+= app_bundle qt release warn_on
DEFINES		+= QT_DEPRECATED_WARNINGS
ICON            = Images/chess.icns
LANGUAGE        = C++
QT		+= network opengl
TEMPLATE        = app

QMAKE_CLEAN	+= QtChess
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
QMAKE_DISTCLEAN += .qmake.cache .qmake.stash
QMAKE_EXTRA_TARGETS = purge
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7

LIBS            += -framework Cocoa
RESOURCES       = Images/images.qrc

FORMS		= UI/qtchess_helpDialog.ui \
                  UI/qtchess_mainwindow.ui \
                  UI/qtchess_promotionDialog.ui \
                  UI/qtchess_setupDialog.ui

HEADERS		= Source/qtchess.h \
                  Source/qtchess_comm.h \
                  Source/qtchess_defs.h \
                  Source/qtchess_gui.h \
                  Source/qtchess_gui_opengl.h \
                  Source/qtchess_validate.h

OBJECTIVE_HEADERS += Source/Cocoainitializer.h
OBJECTIVE_SOURCES += Source/Cocoainitializer.mm

SOURCES		= Source/qtchess.cc \
                  Source/qtchess_comm.cc \
                  Source/qtchess_gui.cc \
                  Source/qtchess_gui_opengl.cc \
                  Source/qtchess_main.cc \
                  Source/qtchess_validate.cc

PROJECTNAME	= QtChess
TARGET		= QtChess

macdeployqt.path  = QtChess.app
macdeployqt.extra = $$[QT_INSTALL_BINS]/macdeployqt ./QtChess.app

INSTALLS	= macdeployqt
