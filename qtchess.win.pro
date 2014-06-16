purge.commands = del *~

CONFIG	        += qt release warn_on
ICON            = chess.png
LANGUAGE        = C++
RC_FILE		= qtchess.win.rc
QT		+= network opengl
TEMPLATE	= app

QMAKE_CLEAN	+= QtChess
QMAKE_CXXFLAGS  += -Wall -Wcast-align -Wcast-qual -Werror -Wextra \
                   -Wformat=2 -Woverloaded-virtual -Wpointer-arith \
                   -Wstrict-overflow=4 -fwrapv -pie
QMAKE_EXTRA_TARGETS = purge

FORMS	=	helpDialog.ui \
		mainwindow.ui \
                promotionDialog.ui \
                setupDialog.ui

HEADERS	=	qtchess.h \
                qtchess_comm.h \
                qtchess_defs.h \
                qtchess_gui.h \
                qtchess_gui_opengl.h \
                qtchess_validate.h

SOURCES	=	qtchess.cc \
                qtchess_comm.cc \
                qtchess_gui.cc \
                qtchess_gui_opengl.cc \
                qtchess_main.cc \
                qtchess_validate.cc

PROJECTNAME	= QtChess
TARGET		= QtChess
