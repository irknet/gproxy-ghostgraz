# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Release/Qt_MinGW-Windows
TARGET = GProxy
VERSION = 2.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
QT = core gui network
SOURCES += source/thread/GproxyUpdateThread.cpp source/socket.cpp source/thread/DownloadThread.cpp source/GhostGrazLogininformationDialog.cpp tests/newsimpletest.cpp source/statspage/Statspage.cpp source/bnetprotocol.cpp source/bnet.cpp source/Player.cpp source/ConfigGUI.cpp source/util.cpp source/Config.cpp source/gameprotocol.cpp source/delegate/GamelistDelegate.cpp source/delegate/ChannellistDelegate.cpp source/OutputareaColorDialog.cpp source/MainGUI.cpp tests/TestClass.cpp source/commandpacket.cpp source/gpsprotocol.cpp source/gproxy.cpp source/widgets/ClickableLineEdit.cpp source/bncsutilinterface.cpp
HEADERS += source/commandpacket.h source/gproxy.h source/ConfigGUI.h source/GhostGrazLogininformationDialog.h source/thread/GproxyUpdateThread.h source/gpsprotocol.h source/bnet.h source/gameprotocol.h source/statspage/Statspage.h source/bnetprotocol.h source/util.h source/Config.h tests/TestClass.h source/MainGUI.h source/delegate/ChannellistDelegate.h source/OutputareaColorDialog.h source/Player.h source/thread/DownloadThread.h source/delegate/GamelistDelegate.h source/socket.h source/bncsutilinterface.h source/widgets/ClickableLineEdit.h
FORMS += source/GhostGrazLogininformationDialog.ui source/MainGUI.ui source/OutputareaColorDialog.ui source/ConfigGUI.ui
RESOURCES += source/resources.qrc
TRANSLATIONS +=
OBJECTS_DIR = build/Release/Qt_MinGW-Windows
MOC_DIR = source
RCC_DIR = source
UI_DIR = source
QMAKE_CC = mingw32-gcc.exe
QMAKE_CXX = mingw32-g++.exe
DEFINES += 
INCLUDEPATH += source ../BNCSUtil/src . . . . . 
LIBS += source/ms_sdk/version.lib source/ms_sdk/ws2_32.lib -L../BNCSUtil/dist/Release/Qt_MinGW-Windows ../BNCSUtil/dist/Release/Qt_MinGW-Windows/BNCSUtil.dll  
