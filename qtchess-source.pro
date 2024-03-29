FORMS		= UI/qtchess_help.ui \
                  UI/qtchess_mainwindow.ui \
                  UI/qtchess_promotion.ui \
                  UI/qtchess_setup.ui

HEADERS		= Source/qtchess.h \
                  Source/qtchess_communications.h \
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
                  Source/qtchess_communications.cc \
                  Source/qtchess_gui.cc \
                  Source/qtchess_gui_board.cc \
                  Source/qtchess_main.cc \
                  Source/qtchess_validate.cc
