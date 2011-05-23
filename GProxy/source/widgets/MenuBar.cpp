#include "MenuBar.h"
#include <QIcon>

MenuBar::MenuBar (MainGUI *mainGUI) : QMenuBar(mainGUI)
{
    this->mainGUI = mainGUI;

    fileMenu = this->addMenu("File");
    configAction = new QAction(QIcon(":/images/Tool.png"), "Options", fileMenu);
    exitAction = new QAction(QIcon(":/images/Cancel.png"), "Exit", fileMenu);
    fileMenu->addAction(configAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    startWarcraftAction = this->addAction("Start Warcraft");

    connect(configAction, SIGNAL(activated()),
            mainGUI, SLOT(showConfigDialog()));
    connect(exitAction, SIGNAL(activated()),
            this, SLOT(onExitAction()));
    connect(startWarcraftAction, SIGNAL(activated()),
            this, SLOT(onWarcraftAction()));
}

MenuBar::~MenuBar ()
{
    delete fileMenu;
//    delete configAction;
//    delete exitAction;
    delete startWarcraftAction;
}

void MenuBar::onExitAction()
{
    mainGUI->close();
}

void MenuBar::onWarcraftAction()
{
    mainGUI->startWarcraft();
}