#ifndef _GHOSTGRAZLOGININFORMATIONDIALOG_H
#define	_GHOSTGRAZLOGININFORMATIONDIALOG_H

#include "ui_GhostGrazLogininformationDialog.h"
#include "Config.h"
#include "Statspage.h"

class GhostGrazLogininformationDialog : public QDialog
{
    Q_OBJECT

public:
    GhostGrazLogininformationDialog(Config *config, Statspage *statspage);
    virtual ~GhostGrazLogininformationDialog();

    QString getUsername();
    QString getPassword();

private:
    Ui::GhostGrazLogininformationDialog widget;
    Config *config;
    Statspage *statspage;

private slots:
    void accept();
    void reject();
};

#endif	/* _GHOSTGRAZLOGININFORMATIONDIALOG_H */
