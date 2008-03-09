TEMPLATE	= lib
LANGUAGE	= C++
CONFIG		+= qt warn_on release plugin static
QT		+= network opengl
QMAKE_CLEAN	+= QtChess
DEFINES		+= QTCHESS_PLUGIN
QMAKE_CXXFLAGS  += -Wall -Werror

FORMS		=	helpDialog.ui \
			mainwindow.ui \
			promotionDialog.ui

HEADERS		=	qtchess.h \
			qtchess_gui.h \
			qtchess_comm.h \
			qtchess_defs.h \
			qtchess_validate.h \
			qtchess_gui_opengl.h

SOURCES		=	qtchess.cc \
			qtchess_gui.cc \
			qtchess_comm.cc \
			qtchess_main.cc \
			qtchess_validate.cc \
			qtchess_gui_opengl.cc

TARGET		= QtChess
PROJECTNAME	= QtChess
