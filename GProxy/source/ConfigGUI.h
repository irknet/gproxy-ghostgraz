#ifndef _CONFIGGUI_H
#define	_CONFIGGUI_H

#include "ui_ConfigGUI.h"
#include "Config.h"
#include "MainGUI.h"
#include "widget/MButton.h"
#include "widget/MColorDialog.h"

#include <QLineEdit>
#include <QComboBox>
#include <QPalette>
#include <QColor>
#include <QFont>

class ConfigGUI : public QDialog
{
    Q_OBJECT

public:
    ConfigGUI(MainGUI* mainGUI);
    virtual ~ConfigGUI();

signals:
    void applyConfig();
    void colorChanged(const QString&, const QColor&);
    void fontChanged(const QString&, const QFont&);

private:
    Ui::ConfigGUI widget;
    MainGUI* mainGUI;
    Config* config;

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
    void onBackgroundcolorButtonClicked();
    void onBackgroundColorChanged(const QColor& color);
    void onOutputareaFontButtonClicked();
    void onOutputareaFontChanged(const QFont& font);
    void onForegroundcolorButtonClicked(MButton* button);
    void onForegroundColorChanged(MColorDialog* colorDialog);
};

#endif	/* _CONFIGGUI_H */
