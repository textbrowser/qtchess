CONFIG	        += qt release warn_on
ICON            = chess.png
LANGUAGE        = C++
QT		+= network opengl
QMAKE_CLEAN	+= QtChess
QMAKE_CXXFLAGS  += -Wall -Wcast-align -Wcast-qual -Werror -Wextra \
                   -Woverloaded-virtual -Wpointer-arith \
                   -Wstrict-overflow=4 -fwrapv -pie
RC_FILE	= qtchess.win.rc
TEMPLATE        = app

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

TARGET	        = QtChess
PROJECTNAME	= QtChess
