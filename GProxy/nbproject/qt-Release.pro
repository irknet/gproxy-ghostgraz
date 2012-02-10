# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Release/Qt_MinGW-Windows
TARGET = GProxy
VERSION = 2.3.2
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
QT = core gui network
SOURCES += source/thread/GproxyUpdateThread.cpp source/socket.cpp source/data/Friend.cpp source/GhostGrazLogininformationDialog.cpp source/util/ColorUtil.cpp source/widget/ClickableListWidget.cpp source/widget/ClickableLineEdit.cpp source/GameListDownloader.cpp source/bnetprotocol.cpp source/bnet.cpp source/thread/SleepThread.cpp source/ConfigGUI.cpp source/data/Player.cpp source/Config.cpp source/gameprotocol.cpp source/delegate/GamelistDelegate.cpp source/delegate/ChannellistDelegate.cpp source/MainGUI.cpp source/widget/MTextEdit.cpp source/commandpacket.cpp source/data/Product.cpp source/widget/MButton.cpp source/Statspage.cpp source/widget/MColorDialog.cpp source/util/Util.cpp source/data/Slot.cpp source/data/GameListEntry.cpp source/gpsprotocol.cpp source/data/ColoredMessage.cpp source/gproxy.cpp source/bncsutilinterface.cpp
HEADERS += source/commandpacket.h source/widget/MTextEdit.h source/Statspage.h source/gproxy.h source/ConfigGUI.h source/util/Util.h source/GhostGrazLogininformationDialog.h source/thread/GproxyUpdateThread.h source/gpsprotocol.h source/widget/MColorDialog.h source/widget/MButton.h source/bnet.h source/util/ColorUtil.h source/GameListDownloader.h source/data/Player.h source/gameprotocol.h source/widget/ClickableLineEdit.h source/data/Product.h source/bnetprotocol.h source/data/ColoredMessage.h source/Config.h source/data/Friend.h source/util/VariantUtil.h source/thread/SleepThread.h source/data/Slot.h source/data/GameListEntry.h source/MainGUI.h source/delegate/ChannellistDelegate.h source/delegate/GamelistDelegate.h source/widget/ClickableListWidget.h source/socket.h source/bncsutilinterface.h
FORMS += source/GhostGrazLogininformationDialog.ui source/MainGUI.ui source/ConfigGUI.ui
RESOURCES += source/resources.qrc
TRANSLATIONS +=
OBJECTS_DIR = build/Release/Qt_MinGW-Windows
MOC_DIR = source
RCC_DIR = source
UI_DIR = source
QMAKE_CC = gcc.exe
QMAKE_CXX = g++.exe
DEFINES += 
INCLUDEPATH += source ../BNCSUtil/src . . . . . . . . . . . . . . . . 
LIBS += source/ms_sdk/version.lib source/ms_sdk/ws2_32.lib -L../BNCSUtil/dist/Release/Qt_MinGW-Windows ../BNCSUtil/dist/Release/Qt_MinGW-Windows/BNCSUtil.dll  
