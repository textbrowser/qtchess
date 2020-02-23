purge.commands = rm -f *~ && rm -f */*~

CONFIG		+= qt release warn_on
DEFINES		+= QT_DEPRECATED_WARNINGS
LANGUAGE        = C++
QT		+= network opengl
TEMPLATE        = app

QMAKE_CLEAN	+= QtChess
QMAKE_CXXFLAGS  += -Wall -Wcast-align -Wcast-qual \
                   -Werror -Wextra -Wformat=2 \
		   -Wno-deprecated-copy \
                   -Woverloaded-virtual -Wpointer-arith \
                   -Wstack-protector -Wstrict-overflow=5 \
                   -fPIE -fstack-protector-all -fwrapv \
                   -mtune=generic -pie -std=c++11
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

PROJECTNAME	= QtChess
TARGET		= QtChess
