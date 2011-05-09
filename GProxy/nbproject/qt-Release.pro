# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Release/Qt_MinGW-Windows
TARGET = GProxy
VERSION = 2.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
QT = core gui network
SOURCES += source/socket.cpp source/bnetprotocol.cpp source/DownloadThread.cpp source/GProxyUpdateThread.cpp source/bnet.cpp source/ConfigGUI.cpp source/util.cpp source/Config.cpp source/gameprotocol.cpp source/delegate/ChannellistDelegate.cpp source/delegate/GamelistDelegate.cpp source/MainGUI.cpp source/commandpacket.cpp source/config_.cpp source/gpsprotocol.cpp source/gproxy.cpp source/bncsutilinterface.cpp
HEADERS += source/commandpacket.h source/gproxy.h source/ConfigGUI.h source/gpsprotocol.h source/bnet.h source/config_.h source/gameprotocol.h source/bnetprotocol.h source/Config.h source/util.h source/GProxyUpdateThread.h source/DownloadThread.h source/MainGUI.h source/delegate/ChannellistDelegate.h source/delegate/GamelistDelegate.h source/socket.h source/bncsutilinterface.h
FORMS += source/MainGUI.ui source/ConfigGUI.ui
RESOURCES += source/images.qrc
TRANSLATIONS +=
OBJECTS_DIR = build/Release/Qt_MinGW-Windows
MOC_DIR = source
RCC_DIR = source
UI_DIR = source
QMAKE_CC = mingw32-gcc.exe
QMAKE_CXX = mingw32-g++.exe
DEFINES += 
INCLUDEPATH += source ../BNCSUtil/src 
LIBS += source/ms_sdk/version.lib source/ms_sdk/winmm.lib source/ms_sdk/ws2_32.lib -L../BNCSUtil/dist/Release/Qt_MinGW-Windows ../BNCSUtil/dist/Release/Qt_MinGW-Windows/BNCSUtil.dll  
