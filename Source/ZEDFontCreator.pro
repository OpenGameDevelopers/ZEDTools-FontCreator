QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET_NAME = ZEDFontCreator
DESTDIR = ../Bin

QMAKE_CXXFLAGS += -std=c++11

release {
TARGET = $${TARGET_NAME}
OBJECTS_DIR = ../Obj/Release
MOC_DIR = ../Obj/Release
QMAKE_CXXFLAGS += -DBUILD_RELEASE
}

debug {
TARGET = $${TARGET_NAME}D
OBJECTS_DIR = ../Obj/Debug
MOC_DIR = ../Obj/Debug
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS += -g -ggdb -DBUILD_DEBUG
}

TEMPLATE = app


SOURCES +=\
    Source/MainWindow.cpp \
    Source/Main.cpp \
    Source/FontWidget.cpp \
    Source/Targa.cpp \
    Source/FontFile.cpp


INCLUDEPATH += ./Headers $(FREETYPE_INC)
LIBPATH += $(FREETYPE_LIB)

LIBS += -lfreetype

HEADERS  +=\
	Headers/MainWindow.h \
	Headers/FontWidget.h \
	Headers/Targa.h \
	Headers/FontFile.h

versioninfo.target = Headers/GitVersion.h
versioninfo.commands = @mkdir -p Headers;\
    ./GitVersion.sh ./Headers/GitVersion.h $(TARGET_NAME)
versioninfo.depends = ../.git

QMAKE_EXTRA_TARGETS = versioninfo

PRE_TARGETDEPS += Headers/GitVersion.h
