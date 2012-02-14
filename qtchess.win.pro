TEMPLATE	= app
LANGUAGE	= C++
CONFIG	+= qt warn_on release
QT		+= network opengl
QMAKE_CLEAN	+= QtChess
QMAKE_CXXFLAGS  += -Wall -Wextra -Werror
ICON		= chess.png
RC_FILE	= qtchess.win.rc

FORMS		=	helpDialog.ui \
			mainwindow.ui \
			setupDialog.ui \
			promotionDialog.ui

HEADERS	=	qtchess.h \
			qtchess_gui.h \
			qtchess_comm.h \
			qtchess_defs.h \
			qtchess_validate.h \
			qtchess_gui_opengl.h

SOURCES	=	qtchess.cc \
			qtchess_gui.cc \
			qtchess_comm.cc \
			qtchess_main.cc \
			qtchess_validate.cc \
			qtchess_gui_opengl.cc

TARGET	= QtChess
PROJECTNAME	= QtChess
