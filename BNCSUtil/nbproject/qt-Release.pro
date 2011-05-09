# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = lib
DESTDIR = dist/Release/Qt_MinGW-Windows
TARGET = BNCSUtil
VERSION = 
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += dll release 
QT = 
SOURCES += src/bncsutil/decodekey.cpp src/bncsutil/cdkeydecoder.cpp src/bncsutil/checkrevision.cpp src/bncsutil/pe.c src/bncsutil/nls.c src/bncsutil/bsha1.cpp src/bncsutil/file.cpp src/bncsutil/stack.c src/bncsutil/oldauth.cpp src/bncsutil/sha1.c src/bncsutil/libinfo.cpp
HEADERS += src/bncsutil/libinfo.h src/bncsutil/file.h src/bncsutil/checkrevision.h src/bncsutil/mutil_types.h src/bncsutil/pe.h src/bncsutil/keytables.h src/bncsutil/buffer.h src/bncsutil/sha1.h src/bncsutil/gmp.h src/bncsutil/nls.h src/bncsutil/cdkeydecoder.h src/bncsutil/ms_stdint.h src/bncsutil/oldauth.h src/bncsutil/mutil.h src/bncsutil/bncsutil.h src/bncsutil/stack.h src/bncsutil/decodekey.h src/bncsutil/bsha1.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Release/Qt_MinGW-Windows
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = mingw32-gcc.exe
QMAKE_CXX = mingw32-g++.exe
DEFINES += 
INCLUDEPATH += src 
LIBS += src/libgmp.a src/version.lib  
