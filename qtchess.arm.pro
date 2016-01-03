purge.commands = rm -f *~ && rm -f */*~

CONFIG		+= qt release warn_on
LANGUAGE        = C++
QT		+= network opengl
TEMPLATE        = app

QMAKE_CLEAN	+= QtChess
QMAKE_CXXFLAGS  += -Wall -Wcast-qual \
                   -Werror -Wextra -Wformat=2 \
                   -Woverloaded-virtual -Wpointer-arith \
                   -Wstack-protector -Wstrict-overflow=5 \
                   -fPIE -fstack-protector-all -fwrapv \
                   -march=armv6 -mfloat-abi=hard -mfpu=vfp \
		   -pie
QMAKE_EXTRA_TARGETS = purge

INCLUDEPATH     += Include
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

SOURCES		= Source/qtchess.cc \
                  Source/qtchess_comm.cc \
                  Source/qtchess_gui.cc \
                  Source/qtchess_gui_opengl.cc \
                  Source/qtchess_main.cc \
                  Source/qtchess_validate.cc

PROJECTNAME	= QtChess
TARGET		= QtChess