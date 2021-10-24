purge.commands = rm -f *~ && rm -f */*~

CONFIG		+= qt release warn_on
DEFINES		+= QT_DEPRECATED_WARNINGS
LANGUAGE        = C++
QT		+= qui network widgets
TEMPLATE        = app

QMAKE_CLEAN	+= QtChess
QMAKE_CXXFLAGS  += -Wall \
                   -Wcast-qual \
                   -Wconversion \
                   -Werror \
                   -Wextra \
                   -Wformat=2 \
                   -Wno-deprecated-copy \
                   -Woverloaded-virtual \
                   -Wpointer-arith \
                   -Wsign-conversion \
                   -Wstack-protector \
                   -Wstrict-overflow=5 \
                   -fPIE \
                   -fstack-protector-all \
                   -fwrapv \
                   -march=armv6 \
                   -mfloat-abi=hard \
                   -mfpu=vfp \
                   -pedantic \
		   -pie
QMAKE_EXTRA_TARGETS = purge

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

SOURCES		= Source/qtchess.cc \
                  Source/qtchess_comm.cc \
                  Source/qtchess_gui.cc \
                  Source/qtchess_gui_opengl.cc \
                  Source/qtchess_main.cc \
                  Source/qtchess_validate.cc

PROJECTNAME	= QtChess
TARGET		= QtChess
