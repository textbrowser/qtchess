FORMS		= UI/qtchess_helpDialog.ui \
                  UI/qtchess_mainwindow.ui \
                  UI/qtchess_promotionDialog.ui \
                  UI/qtchess_setupDialog.ui

HEADERS		= Source/qtchess.h \
                  Source/qtchess_comm.h \
                  Source/qtchess_definitions.h \
                  Source/qtchess_gui.h \
                  Source/qtchess_gui_board.h \
                  Source/qtchess_validate.h

mac {
LIBS            += -framework Cocoa

OBJECTIVE_HEADERS += Source/Cocoainitializer.h
OBJECTIVE_SOURCES += Source/Cocoainitializer.mm
}

RESOURCES       = Images/images.qrc

SOURCES		= Source/qtchess.cc \
                  Source/qtchess_comm.cc \
                  Source/qtchess_gui.cc \
                  Source/qtchess_gui_board.cc \
                  Source/qtchess_main.cc \
                  Source/qtchess_validate.cc
