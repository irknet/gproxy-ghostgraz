#include "GhostGrazLogininformationDialog.h"

GhostGrazLogininformationDialog::GhostGrazLogininformationDialog (Config *config, Statspage *statspage)
{
    widget.setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint
            | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    this->config = config;
    this->statspage = statspage;
}

GhostGrazLogininformationDialog::~GhostGrazLogininformationDialog () { }

QString GhostGrazLogininformationDialog::getUsername()
{
    return widget.usernameTextfield->text();
}

QString GhostGrazLogininformationDialog::getPassword()
{
    return widget.passwordTextfield->text();
}

void GhostGrazLogininformationDialog::accept ()
{
    config->setString("ghostgrazUsername", getUsername());
    config->setString("ghostgrazPassword", getPassword());
    config->commit();
    statspage->login(getUsername(), getPassword());

    done(QDialog::Accepted);
}

void GhostGrazLogininformationDialog::reject ()
{
    statspage->login("", "");
    done(QDialog::Rejected);
}