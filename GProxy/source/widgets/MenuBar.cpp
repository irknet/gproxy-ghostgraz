#include "MenuBar.h"
#include "ConfigGUI.h"

#include <QIcon>

MenuBar::MenuBar (MainGUI *mainGUI) : QMenuBar(mainGUI)
{
    this->mainGUI = mainGUI;

    QMenu *fileMenu = this->addMenu("File");
    QAction *configAction = new QAction(QIcon(":/images/Tool.png"), "Config", fileMenu);
    QAction *exitAction = new QAction(QIcon(":/images/Cancel.png"), "Exit", fileMenu);
    fileMenu->addAction(configAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    QAction *startWarcraftAction = this->addAction("Start Warcraft");

    connect(configAction, SIGNAL(activated()),
            this, SLOT(onConfigAction()));
    connect(exitAction, SIGNAL(activated()),
            this, SLOT(onExitAction()));
    connect(startWarcraftAction, SIGNAL(activated()),
            this, SLOT(onWarcraftAction()));
}

MenuBar::~MenuBar () { }

void MenuBar::onConfigAction()
{
    ConfigGUI* config = new ConfigGUI(mainGUI->getGproxy()->getConfig());
    config->exec();
}

void MenuBar::onExitAction()
{
    mainGUI->close();
}

void MenuBar::onWarcraftAction()
{
    mainGUI->startWarcraft();
}