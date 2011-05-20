#ifndef MENUBAR_H
#define	MENUBAR_H

#include "MainGUI.h"
#include <QMenuBar>

class MenuBar : public QMenuBar
{
    Q_OBJECT

public:
    MenuBar(MainGUI *mainGUI);
    virtual ~MenuBar();

private:
    MainGUI *mainGUI;

private slots:
    void onConfigAction();
    void onExitAction();
    void onWarcraftAction();
};

#endif	/* MENUBAR_H */

