HEADERS += \
    $$PWD/chess.h \
    $$PWD/palette.h \
    $$PWD/resmgr.h

SOURCES += \
    $$PWD/chess.cpp \
    $$PWD/main.cpp \
    $$PWD/palette.cpp \
    $$PWD/resmgr.cpp
	
include(board/board.pri)
include(util/util.pri)
