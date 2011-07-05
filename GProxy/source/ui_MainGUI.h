/********************************************************************************
** Form generated from reading UI file 'MainGUI.ui'
**
** Created: Tue 5. Jul 20:05:16 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINGUI_H
#define UI_MAINGUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>
#include "widget/ClickableListWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainGUI
{
public:
    QAction *actionOptions;
    QAction *actionExit;
    QWidget *centralwidget;
    QTextEdit *outputField;
    QPlainTextEdit *inputField;
    ClickableListWidget *channelList;
    ClickableListWidget *friendList;
    ClickableListWidget *gameList;
    QLineEdit *channelField;
    QPushButton *refreshButton;
    QMenuBar *menuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *MainGUI)
    {
        if (MainGUI->objectName().isEmpty())
            MainGUI->setObjectName(QString::fromUtf8("MainGUI"));
        MainGUI->resize(1085, 555);
        MainGUI->setMinimumSize(QSize(500, 200));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/GProxy.gif"), QSize(), QIcon::Normal, QIcon::Off);
        MainGUI->setWindowIcon(icon);
        actionOptions = new QAction(MainGUI);
        actionOptions->setObjectName(QString::fromUtf8("actionOptions"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/Tool.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOptions->setIcon(icon1);
        actionExit = new QAction(MainGUI);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/Cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon2);
        centralwidget = new QWidget(MainGUI);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        QPalette palette;
        QBrush brush(QColor(230, 230, 230, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        QBrush brush2(QColor(120, 120, 120, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        QBrush brush3(QColor(255, 255, 255, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        centralwidget->setPalette(palette);
        QFont font;
        font.setFamily(QString::fromUtf8("Courier New"));
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        centralwidget->setFont(font);
        outputField = new QTextEdit(centralwidget);
        outputField->setObjectName(QString::fromUtf8("outputField"));
        outputField->setEnabled(true);
        outputField->setGeometry(QRect(0, 0, 661, 521));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(9);
        font1.setBold(false);
        font1.setWeight(50);
        outputField->setFont(font1);
        outputField->setFocusPolicy(Qt::ClickFocus);
        outputField->setAutoFillBackground(true);
        outputField->setReadOnly(true);
        outputField->setAcceptRichText(false);
        outputField->setTextInteractionFlags(Qt::TextSelectableByMouse);
        inputField = new QPlainTextEdit(centralwidget);
        inputField->setObjectName(QString::fromUtf8("inputField"));
        inputField->setGeometry(QRect(0, 521, 661, 31));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Courier New"));
        font2.setPointSize(10);
        inputField->setFont(font2);
        inputField->setStyleSheet(QString::fromUtf8(""));
        channelList = new ClickableListWidget(centralwidget);
        channelList->setObjectName(QString::fromUtf8("channelList"));
        channelList->setGeometry(QRect(900, 20, 181, 301));
        channelList->setContextMenuPolicy(Qt::CustomContextMenu);
        channelList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        channelList->setSelectionMode(QAbstractItemView::SingleSelection);
        friendList = new ClickableListWidget(centralwidget);
        friendList->setObjectName(QString::fromUtf8("friendList"));
        friendList->setGeometry(QRect(900, 330, 181, 221));
        friendList->setContextMenuPolicy(Qt::CustomContextMenu);
        friendList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        friendList->setSelectionMode(QAbstractItemView::NoSelection);
        gameList = new ClickableListWidget(centralwidget);
        gameList->setObjectName(QString::fromUtf8("gameList"));
        gameList->setGeometry(QRect(665, 30, 231, 521));
        gameList->setContextMenuPolicy(Qt::NoContextMenu);
        gameList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        gameList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        gameList->setSelectionMode(QAbstractItemView::SingleSelection);
        channelField = new QLineEdit(centralwidget);
        channelField->setObjectName(QString::fromUtf8("channelField"));
        channelField->setGeometry(QRect(900, 0, 181, 21));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        channelField->setPalette(palette1);
        QFont font3;
        font3.setFamily(QString::fromUtf8("Courier New"));
        font3.setPointSize(12);
        font3.setBold(true);
        font3.setWeight(75);
        channelField->setFont(font3);
        channelField->setCursor(QCursor(Qt::BlankCursor));
        channelField->setAlignment(Qt::AlignCenter);
        channelField->setReadOnly(true);
        refreshButton = new QPushButton(centralwidget);
        refreshButton->setObjectName(QString::fromUtf8("refreshButton"));
        refreshButton->setGeometry(QRect(670, 0, 221, 23));
        MainGUI->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(MainGUI);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1085, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainGUI->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOptions);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);

        retranslateUi(MainGUI);
        QObject::connect(actionExit, SIGNAL(triggered()), MainGUI, SLOT(close()));
        QObject::connect(gameList, SIGNAL(clicked(QMouseEvent*)), MainGUI, SLOT(onGameListItemClicked(QMouseEvent*)));
        QObject::connect(inputField, SIGNAL(textChanged()), MainGUI, SLOT(onInputFieldTextChanged()));
        QObject::connect(channelField, SIGNAL(textChanged(QString)), MainGUI, SLOT(onChannelChanged()));
        QObject::connect(channelList, SIGNAL(customContextMenuRequested(QPoint)), MainGUI, SLOT(onChannelContextMenu(QPoint)));
        QObject::connect(friendList, SIGNAL(customContextMenuRequested(QPoint)), MainGUI, SLOT(onFriendsContextMenu(QPoint)));
        QObject::connect(refreshButton, SIGNAL(clicked()), MainGUI, SLOT(onRefreshButtonClicked()));
        QObject::connect(channelList, SIGNAL(clicked(QMouseEvent*)), MainGUI, SLOT(onChannellistItemClicked(QMouseEvent*)));
        QObject::connect(friendList, SIGNAL(clicked(QMouseEvent*)), MainGUI, SLOT(onFriendlistItemClicked(QMouseEvent*)));
        QObject::connect(actionOptions, SIGNAL(triggered()), MainGUI, SLOT(showConfigDialog()));

        QMetaObject::connectSlotsByName(MainGUI);
    } // setupUi

    void retranslateUi(QMainWindow *MainGUI)
    {
        MainGUI->setWindowTitle(QApplication::translate("MainGUI", "GProxy GhostGraz v2.2", 0, QApplication::UnicodeUTF8));
        actionOptions->setText(QApplication::translate("MainGUI", "Options", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainGUI", "Exit", 0, QApplication::UnicodeUTF8));
        outputField->setHtml(QApplication::translate("MainGUI", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Arial'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Terminal'; font-size:12pt; font-weight:600;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
        inputField->setPlainText(QString());
        channelField->setText(QApplication::translate("MainGUI", "Channel", 0, QApplication::UnicodeUTF8));
        refreshButton->setText(QApplication::translate("MainGUI", "Refresh", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainGUI", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainGUI: public Ui_MainGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINGUI_H
