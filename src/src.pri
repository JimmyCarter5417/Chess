HEADERS += \
    $$PWD/chess.h \
    $$PWD/co.h \
    $$PWD/debug.h \
    $$PWD/palette.h \
    $$PWD/resmgr.h \
    $$PWD/size.h

SOURCES += \
    $$PWD/chess.cpp \
    $$PWD/debug.cpp \
    $$PWD/main.cpp \
    $$PWD/palette.cpp \
    $$PWD/resmgr.cpp
	
include(algo/algo.pri)
include(model/model.pri)
include(include/include.pri)
