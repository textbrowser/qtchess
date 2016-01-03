cache()
purge.commands = rm -f *~ && rm -f */*~

CONFIG		+= app_bundle qt release warn_on
ICON            = Images/chess.icns
LANGUAGE        = C++
QT		+= network opengl
TEMPLATE        = app

QMAKE_CLEAN	+= QtChess
QMAKE_CXXFLAGS  += -Wall -Wcast-align -Wcast-qual \
                   -Werror -Wextra -Wformat=2 \
                   -Woverloaded-virtual -Wpointer-arith \
                   -Wstack-protector -Wstrict-overflow=5 \
                   -fPIE -fstack-protector-all -fwrapv \
                   -mtune=generic
QMAKE_DISTCLEAN += .qmake.cache .qmake.stash
QMAKE_EXTRA_TARGETS = purge
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6

INCLUDEPATH     += Include
LIBS            += -framework Cocoa
RESOURCES       = Images/images.qrc

FORMS		= UI/helpDialog.ui \
                  UI/mainwindow.ui \
                  UI/promotionDialog.ui \
                  UI/setupDialog.ui

HEADERS		= Include/qtchess.h \
                  Include/qtchess_comm.h \
                  Include/qtchess_defs.h \
                  Include/qtchess_gui.h \
                  Include/qtchess_gui_opengl.h \
                  Include/qtchess_validate.h

OBJECTIVE_HEADERS += Include/Cocoainitializer.h
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