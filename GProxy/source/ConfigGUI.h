#ifndef _CONFIGGUI_H
#define	_CONFIGGUI_H

#include "ui_ConfigGUI.h"
#include "Config.h"
#include "widgets/ClickableLineEdit.h"

#include <QLineEdit>
#include <QComboBox>
#include <QSignalMapper>

class ConfigGUI : public QDialog
{
    Q_OBJECT

public:
    ConfigGUI(Config *cfg);
    ConfigGUI(Config *cfg, bool exitOnClose);
    virtual ~ConfigGUI();

private:
    Ui::ConfigGUI widget;
    Config *cfg;
    ClickableLineEdit *war3pathTextfield;
    bool exitOnClose;

    void init(Config *cfg, bool exitOnClose);
    void initValues();
    void initSlots();
    void setSelectedCBValue(QComboBox *combobox, const QString &value);
    void showErrorMessage(const QString &errorMessage);

private slots:
    void accept();
    void reject();
    void onCDKeyROCChanged(QString text);
    void onCDKeyTFTChanged(QString text);
    void onUsernameChanged(QString text);
    void onPasswordChanged(QString text);
    void onChannelChanged(QString text);
    void onWar3pathChangeRequest();
    void onServerComboboxItemChanged(const QString &text);
    void onForegroundcolorButtonClicked();
};

#endif	/* _CONFIGGUI_H */
