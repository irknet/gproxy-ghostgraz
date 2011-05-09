#ifndef _CONFIGGUI_H
#define	_CONFIGGUI_H

#include "ui_ConfigGUI.h"
#include "Config.h"
#include <QLineEdit>
#include <QComboBox>
#include <QSignalMapper>

class ConfigGUI : public QDialog
{
    Q_OBJECT

private:
    Ui::ConfigGUI widget;
    Config *cfg;
    QSignalMapper *smTextfieldTextChanged;
    QSignalMapper *smServer;
    QSignalMapper *smFileDialog;

    void init();
    void setSelectedCBValue(QComboBox *combobox, QString value);

public:
    ConfigGUI(Config *cfg);
    virtual ~ConfigGUI();

private slots:
    void accept();
    void reject();
    void onServerCBChanged(QWidget *);
    void onTextfieldTextChanged(QWidget *);
    void onOpenFileDialog(QWidget *);
};

class FileDialogLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    FileDialogLineEdit(QString text, QWidget *parent = 0) : QLineEdit(text, parent) { }

protected:
    void mousePressEvent(QMouseEvent *)
    {
        clearFocus();
        emit clicked();
    }

signals:
    void clicked();
};

#endif	/* _CONFIGGUI_H */
