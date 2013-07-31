TEMPLATE	= app
LANGUAGE	= C++
CONFIG		+= qt warn_on release app_bundle
QT		+= network opengl
QMAKE_CLEAN	+= QtChess
QMAKE_CXXFLAGS  += -Wall -Wextra -Werror -Wstack-protector -fPIE -fstack-protector-all -pie
QMAKE_LFLAGS_RELEASE =
QMAKE_LFLAGS_RPATH =
ICON            = chess.icns

FORMS		=	helpDialog.ui \
			mainwindow.ui \
			setupDialog.ui \
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
