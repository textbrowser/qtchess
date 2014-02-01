CONFIG		+= app_bundle qt release warn_on
ICON            = chess.icns
LANGUAGE        = C++
QT		+= network opengl
TEMPLATE        = app

QMAKE_CLEAN	+= QtChess
QMAKE_CXXFLAGS  += -Wall -Wcast-align -Wcast-qual \
                   -Werror -Wextra \
                   -Woverloaded-virtual -Wpointer-arith \
                   -Wstack-protector -Wstrict-overflow=4 \
                   -fPIE -fstack-protector-all -fwrapv \
                   -mtune=generic -pie -O3

FORMS		= helpDialog.ui \
                  mainwindow.ui \
                  promotionDialog.ui \
                  setupDialog.ui

HEADERS		= qtchess.h \
                  qtchess_comm.h \
                  qtchess_defs.h \
                  qtchess_gui.h \
                  qtchess_gui_opengl.h \
                  qtchess_validate.h

SOURCES		= qtchess.cc \
                  qtchess_comm.cc \
                  qtchess_gui.cc \
                  qtchess_gui_opengl.cc \
                  qtchess_main.cc \
                  qtchess_validate.cc

PROJECTNAME	= QtChess
TARGET		= QtChess
