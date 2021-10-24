purge.commands = del *~ && del */*~

CONFIG	        += qt release warn_on
ICON            = Images\\chess.png
LANGUAGE        = C++
RC_FILE		= qtchess.win.rc
QT		+= gui network opengl
TEMPLATE	= app

QMAKE_CLEAN	+= QtChess
QMAKE_CXXFLAGS  += -Wall \
                   -Wcast-align \
                   -Wcast-qual \
                   -Wextra \
                   -Wformat=2 \
                   -Woverloaded-virtual \
                   -Wpointer-arith \
                   -Wstrict-overflow=5 \
                   -fwrapv \
                   -pedantic \
                   -pie \
                   -std=c++11
QMAKE_EXTRA_TARGETS = purge

RESOURCES = Images\\images.qrc

FORMS	=	UI\\qtchess_helpDialog.ui \
		UI\\qtchess_mainwindow.ui \
                UI\\qtchess_promotionDialog.ui \
                UI\\qtchess_setupDialog.ui

HEADERS	=	Source\\qtchess.h \
                Source\\qtchess_comm.h \
                Source\\qtchess_defs.h \
                Source\\qtchess_gui.h \
                Source\\qtchess_gui_opengl.h \
                Source\\qtchess_validate.h

SOURCES	=	Source\\qtchess.cc \
                Source\\qtchess_comm.cc \
                Source\\qtchess_gui.cc \
                Source\\qtchess_gui_opengl.cc \
                Source\\qtchess_main.cc \
                Source\\qtchess_validate.cc

PROJECTNAME	= QtChess
TARGET		= QtChess
