#include "ConfigGUI.h"
#include "gproxy.h"

#ifdef WIN32
// For reading Warcraft 3 path from registry
#include "windows.h"
#endif

#include <QLabel>
#include <QPushButton>
#include <QToolTip>
#include <QPoint>
#include <QMessageBox>
#include <QIcon>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>

ConfigGUI::ConfigGUI (MainGUI* mainGUI)
{
    widget.setupUi(this);
    this->mainGUI = mainGUI;
    this->config = mainGUI->getGproxy()->getConfig();
//    widget.appearanceTab->setEnabled(false);

    connect(this, SIGNAL(applyConfig()),
            mainGUI->getGproxy(), SLOT(applyConfig()));

    connect(this, SIGNAL(colorChanged(const QString&, const QColor&)),
            mainGUI, SLOT(setColor(const QString&, const QColor&)));

    connect(this, SIGNAL(fontChanged(const QString&, const QFont&)),
            mainGUI, SLOT(setFont(const QString&, const QFont&)));

    initValues();
    initSlots();
}

ConfigGUI::~ConfigGUI () { }

void ConfigGUI::initValues ()
{
    QList<QString> vKey = config->getKeys();
    QList<QString> vValue = config->getValues();

    for (int i = 0; i < vKey.count(); i++)
    {
        QString key = vKey.at(i);
        QString value = vValue.at(i);

        QList<QObject*> tabBarChildren = widget.optionsTabWidget->children().at(0)->children();

        foreach(QObject *tab, tabBarChildren)
        {
            QList<QObject*> tabChildren = tab->children();

            foreach(QObject *tabChild, tabChildren)
            {
                if (tabChild->objectName().startsWith(key))
                {
                    if (qobject_cast<QLineEdit*> (tabChild) != NULL)
                    {
                        QLineEdit *textfield = (QLineEdit*) tabChild;

                        if (key == "war3path" && value.isEmpty())
                        {
#ifdef WIN32
                            string war3path;
                            HKEY hkey;
                            long s = RegOpenKeyExA(HKEY_CURRENT_USER,
                                    "Software\\Blizzard Entertainment\\Warcraft III",
                                    0, KEY_QUERY_VALUE, &hkey);

                            if (s == ERROR_SUCCESS)
                            {
                                char InstallPath[256];
                                DWORD InstallPathSize = 256;
                                RegQueryValueExA(hkey, "InstallPath",
                                        NULL, NULL, (LPBYTE) InstallPath, &InstallPathSize);
                                war3path = InstallPath;
                                RegCloseKey(hkey);

                                textfield->setText(QString::fromStdString(war3path) + QDir::separator());
                            }
#endif
                        }
                        else
                        {
                            textfield->setText(value);
                        }
                    }
                    else if (qobject_cast<QComboBox*> (tabChild) != NULL)
                    {
                        QComboBox *combobox = (QComboBox*) tabChild;
                        if (key == "server")
                        {
                            setSelectedCBValue(combobox, value);
                        }
                        else if (config->getBoolean(key) == false)
                        {
                            combobox->setCurrentIndex(1);
                        }
                    }
                }
            }
        }
    }
}

void ConfigGUI::initSlots ()
{
    connect(widget.cdkeyrocTextfield, SIGNAL(textChanged(const QString &)),
            this, SLOT(onCDKeyROCChanged(QString)));
    connect(widget.cdkeytftTextfield, SIGNAL(textChanged(const QString &)),
            this, SLOT(onCDKeyTFTChanged(QString)));
    connect(widget.usernameTextfield, SIGNAL(textChanged(const QString &)),
            this, SLOT(onUsernameChanged(QString)));
    connect(widget.passwordTextfield, SIGNAL(textChanged(const QString &)),
            this, SLOT(onPasswordChanged(QString)));
    connect(widget.channelTextfield, SIGNAL(textChanged(const QString &)),
            this, SLOT(onChannelChanged(QString)));
    connect(widget.serverCombobox, SIGNAL(activated(const QString &)),
            this, SLOT(onServerComboboxItemChanged(const QString &)));
}

void ConfigGUI::accept ()
{
    QList<QObject*> tabBarChildren = widget.optionsTabWidget->children().at(0)->children();

    foreach(QObject *tab, tabBarChildren)
    {
        QList<QObject*> tabChildren = tab->children();

        foreach(QObject *tabChild, tabChildren)
        {
            if (qobject_cast<QLineEdit*> (tabChild) != NULL)
            {
                QLineEdit *textfield = (QLineEdit*) tabChild;
                QString key = textfield->objectName().remove("Textfield");

                if (!config->setString(key, textfield->text()))
                {
                    showErrorMessage("Could not save " + key + " = " + textfield->text());
                }
            }
            else if (qobject_cast<QComboBox*> (tabChild) != NULL)
            {
                QComboBox *combobox = (QComboBox*) tabChild;
                QString key = combobox->objectName().remove("Combobox");
                if (key == "server")
                {
                    if (combobox->currentText() == "US West (Lordaeron)")
                    {
                        config->setString("server", "uswest.battle.net");
                    }
                    else if (combobox->currentText() == "US East (Azeroth)")
                    {
                        config->setString("server", "useast.battle.net");
                    }
                    else if (combobox->currentText() == "Asia (Kalimdor)")
                    {
                        config->setString("server", "asia.battle.net");
                    }
                    else if (combobox->currentText() == "Europe (Northrend)")
                    {
                        config->setString("server", "europe.battle.net");
                    }
                    else
                    {
                        config->setString("server", combobox->currentText());
                    }
                }
                else
                {
                    if (!config->setString(key, combobox->currentText()))
                    {
                        showErrorMessage("Could not save " + key + " = "
                                + combobox->currentText());
                    }
                }
            }
        }
    }

    if (config->hasRequiredValues())
    {
        config->commit();
        emit applyConfig();
        done(QDialog::Accepted);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowIcon(QIcon(":/images/Error.png"));
        msgBox.setWindowTitle("Error");
        msgBox.setText("You need to fill in all required values to save!");
        msgBox.exec();
    }
}

void ConfigGUI::reject ()
{
    config->loadConfig();
    emit applyConfig();
    done(QDialog::Rejected);
}

void ConfigGUI::onCDKeyROCChanged (QString text)
{
    QPoint position(widget.cdkeyrocTextfield->pos().x() + this->x()
            + widget.cdkeyrocTextfield->width() + 7,
            this->y() + widget.cdkeyrocTextfield->pos().y() + 30);
    int cursorPosition = widget.cdkeyrocTextfield->cursorPosition();

    widget.cdkeyrocTextfield->setText(text.toUpper());
    widget.cdkeyrocTextfield->setCursorPosition(cursorPosition);

    if (text.length() < 26)
    {
        QPalette pal;
        pal.setColor(QPalette::Text, Qt::red);
        widget.cdkeyrocTextfield->setPalette(pal);
        QToolTip::showText(position,
                "The cd key has to be 26 characters long", widget.cdkeyrocTextfield);
    }
    else if (text.length() > 26)
    {
        widget.cdkeyrocTextfield->setText(text.remove(cursorPosition - 1, 1));
        widget.cdkeyrocTextfield->setCursorPosition(cursorPosition - 1);
        QPalette pal;
        pal.setColor(QPalette::Text, Qt::black);
        widget.cdkeyrocTextfield->setPalette(pal);
        QToolTip::showText(position,
                "The cd key has to be 26 characters long", widget.cdkeyrocTextfield);
    }
    else
    {
        QPalette pal;
        pal.setColor(QPalette::Text, Qt::black);
        widget.cdkeyrocTextfield->setPalette(pal);
    }

    if (text.contains("-"))
    {
        widget.cdkeyrocTextfield->setText(text.replace("-", ""));
        QToolTip::showText(position,
                "Enter the cd key without dashes", widget.cdkeyrocTextfield);
    }
}

void ConfigGUI::onCDKeyTFTChanged (QString text)
{
    QPoint position(widget.cdkeytftTextfield->pos().x() + this->x()
            + widget.cdkeytftTextfield->width() + 7,
            this->y() + widget.cdkeytftTextfield->pos().y() + 30);
    int cursorPosition = widget.cdkeytftTextfield->cursorPosition();

    widget.cdkeytftTextfield->setText(text.toUpper());
    widget.cdkeytftTextfield->setCursorPosition(cursorPosition);

    if (text.length() < 26)
    {
        QPalette pal;
        pal.setColor(QPalette::Text, Qt::red);
        widget.cdkeytftTextfield->setPalette(pal);
        QToolTip::showText(position,
                "The cd key has to be 26 characters long", widget.cdkeytftTextfield);
    }
    else if (text.length() > 26)
    {
        widget.cdkeytftTextfield->setText(text.remove(cursorPosition - 1, 1));
        widget.cdkeytftTextfield->setCursorPosition(cursorPosition - 1);
        QPalette pal;
        pal.setColor(QPalette::Text, Qt::black);
        widget.cdkeytftTextfield->setPalette(pal);
        QToolTip::showText(position,
                "The cd key has to be 26 characters long", widget.cdkeytftTextfield);
    }
    else
    {
        QPalette pal;
        pal.setColor(QPalette::Text, Qt::black);
        widget.cdkeytftTextfield->setPalette(pal);
    }

    if (text.contains("-"))
    {
        widget.cdkeytftTextfield->setText(text.replace("-", ""));
        QToolTip::showText(position,
                "Enter the cd key without dashes", widget.cdkeytftTextfield);
    }
}

void ConfigGUI::onUsernameChanged (QString text)
{
    QPoint position(widget.usernameTextfield->pos().x() + this->x()
            + widget.usernameTextfield->width() + 7,
            this->y() + widget.usernameTextfield->pos().y() + 30);
    int cursorPosition = widget.usernameTextfield->cursorPosition();

    if (text.length() < 3)
    {
        QPalette pal;
        pal.setColor(QPalette::Text, Qt::red);
        widget.usernameTextfield->setPalette(pal);
        QToolTip::showText(position,
                "The username has be at least 3 characters long",
                widget.usernameTextfield);
    }
    else if (text.length() > 15)
    {
        widget.usernameTextfield->setText(text.remove(cursorPosition - 1, 1));
        widget.usernameTextfield->setCursorPosition(cursorPosition - 1);
        QPalette pal;
        pal.setColor(QPalette::Text, Qt::black);
        widget.usernameTextfield->setPalette(pal);
        QToolTip::showText(position,
                "The username has a maximum length of 15 characters",
                widget.usernameTextfield);
    }
    else
    {
        QPalette pal;
        pal.setColor(QPalette::Text, Qt::black);
        widget.usernameTextfield->setPalette(pal);
    }
}

void ConfigGUI::onPasswordChanged (QString text)
{
    QPoint position(widget.passwordTextfield->pos().x() + this->x()
            + widget.passwordTextfield->width() + 7,
            this->y() + widget.passwordTextfield->pos().y() + 30);
    int cursorPosition = widget.passwordTextfield->cursorPosition();

    if (text.length() < 3)
    {
        QPalette pal;
        pal.setColor(QPalette::Text, Qt::red);
        widget.passwordTextfield->setPalette(pal);
        QToolTip::showText(position,
                "The password has be at least 3 characters long",
                widget.passwordTextfield);
    }
    else if (text.length() > 15)
    {
        widget.passwordTextfield->setText(text.remove(cursorPosition - 1, 1));
        widget.passwordTextfield->setCursorPosition(cursorPosition - 1);
        QPalette pal;
        pal.setColor(QPalette::Text, Qt::black);
        widget.passwordTextfield->setPalette(pal);
        QToolTip::showText(position,
                "The password has a maximum length of 15 characters",
                widget.passwordTextfield);
    }
    else
    {
        QPalette pal;
        pal.setColor(QPalette::Text, Qt::black);
        widget.passwordTextfield->setPalette(pal);
    }
}

void ConfigGUI::onChannelChanged (QString text)
{
    QPoint position(widget.channelTextfield->pos().x() + this->x()
            + widget.channelTextfield->width() + 7,
            this->y() + widget.channelTextfield->pos().y() + 30);
    int cursorPosition = widget.channelTextfield->cursorPosition();

    if (text.length() > 31)
    {
        widget.channelTextfield->setText(text.remove(cursorPosition - 1, 1));
        widget.channelTextfield->setCursorPosition(cursorPosition - 1);
        QToolTip::showText(position,
                "The channel name has a maximum length of 31 characters",
                widget.channelTextfield);
    }
}

void ConfigGUI::onServerComboboxItemChanged (const QString &text)
{
    if (text == "PvPGN Server")
    {
        QDialog *dialog = new QDialog(this);
        dialog->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint
                | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
        dialog->setWindowTitle("PvPGN Server");
        dialog->setFixedSize(220, 55);

        QLabel *label = new QLabel("Server: ", dialog);
        label->setGeometry(10, 5, 40, 20);

        QLineEdit *textfield = new QLineEdit(dialog);
        textfield->setGeometry(50, 5, 160, 20);
        textfield->setAccessibleName("server");

        QPushButton *button = new QPushButton("Accept", dialog);
        button->setGeometry(10, 30, 200, 20);
        connect(button, SIGNAL(clicked()), dialog, SLOT(accept()));

        if (dialog->exec() == QDialog::Accepted && !textfield->text().isEmpty())
        {
            setSelectedCBValue(widget.serverCombobox, textfield->text());
        }
        else
        {
            setSelectedCBValue(widget.serverCombobox,
                    widget.serverCombobox->accessibleDescription());
        }
    }
    else
    {
        setSelectedCBValue(widget.serverCombobox, widget.serverCombobox->currentText());
    }
}

void ConfigGUI::setSelectedCBValue (QComboBox *combobox, const QString &value)
{
    if (value == "uswest.battle.net" || value == "US West (Lordaeron)")
    {
        combobox->setCurrentIndex(0);
    }
    else if (value == "useast.battle.net" || value == "US East (Azeroth)")
    {
        combobox->setCurrentIndex(1);
    }
    else if (value == "asia.battle.net" || value == "Asia (Kalimdor)")
    {
        combobox->setCurrentIndex(2);
    }
    else if (value == "europe.battle.net" || value == "Europe (Northrend)")
    {
        combobox->setCurrentIndex(3);
    }
    else // TODO if (the PvPGN Server already exists in the combobox)
    {
        if (combobox->count() > 5)
        {
            combobox->removeItem(5);
        }
        combobox->addItem(value);
        combobox->setCurrentIndex(combobox->count() - 1);
    }

    combobox->setAccessibleDescription(combobox->currentText());
}

void ConfigGUI::onWar3pathChangeRequest ()
{
    QFileDialog fileDialog(this);
    fileDialog.setWindowTitle("Choose your Warcraft III Install directory");
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    fileDialog.setDirectory(widget.war3pathTextfield->text());

    if (fileDialog.exec() == QDialog::Accepted)
    {
        widget.war3pathTextfield->setText(QDir::toNativeSeparators(fileDialog.selectedFiles().at(0))
                + QDir::separator());
    }
}

void ConfigGUI::showErrorMessage (const QString &errorMessage)
{
    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(":/images/Error.png"));
    msgBox.setWindowTitle("Error");
    msgBox.setText(errorMessage);
    msgBox.exec();
}

void ConfigGUI::onBackgroundcolorButtonClicked ()
{
    QColorDialog* colorDialog = new QColorDialog(config->getColor("backgroundcolor"), this);

    connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)),
            this, SLOT(onBackgroundColorChanged(const QColor&)), Qt::QueuedConnection);

    if (colorDialog->exec() == QDialog::Accepted)
    {
        QColor backgroundcolor = colorDialog->selectedColor();
        if (backgroundcolor.isValid())
        {
            config->setColor("backgroundcolor", backgroundcolor);
        }
    }
    else
    {
        emit colorChanged("backgroundcolor", config->getColor("backgroundcolor"));
    }
    delete colorDialog;
}

void ConfigGUI::onBackgroundColorChanged(const QColor& color)
{
    emit colorChanged("backgroundcolor", color);
}

void ConfigGUI::onOutputareaFontButtonClicked()
{
    QFontDialog* fontDialog = new QFontDialog(config->getFont("outputareaFont"), this);

    connect(fontDialog, SIGNAL(currentFontChanged(const QFont&)),
            this, SLOT(onOutputareaFontChanged(const QFont&)), Qt::QueuedConnection);

    if (fontDialog->exec() == QDialog::Accepted)
    {
        QFont font = fontDialog->selectedFont();
        config->setFont("outputareaFont", font);
    }
    else
    {
        emit fontChanged("outputarea", config->getFont("outputareaFont"));
    }
    delete fontDialog;
}

void ConfigGUI::onOutputareaFontChanged(const QFont& font)
{
    emit fontChanged("outputarea", font);
}

void ConfigGUI::onForegroundcolorButtonClicked(MButton* button)
{
    QString element = button->objectName();
    // Example name is "generalChatColorButton". Expected element name is "chat".
    // Removing "general" and "ColorButton" and changing case to lower case.
    element = element.mid(7, element.length() - 18).toLower();

    MColorDialog* colorDialog = new MColorDialog(config->getColor(element + "_foregroundcolor"), this);
    colorDialog->setObjectName(element + "ColorDialog");

    connect(colorDialog, SIGNAL(currentColorChanged(MColorDialog*)),
            this, SLOT(onForegroundColorChanged(MColorDialog*)), Qt::QueuedConnection);

    if (colorDialog->exec() == QDialog::Accepted)
    {
        QColor color = colorDialog->selectedColor();
        if (color.isValid())
        {
            config->setColor(element + "_foregroundcolor", color);
        }
    }
    else
    {
        emit colorChanged(element + "_foregroundcolor", config->getColor(element + "_foregroundcolor"));
    }
    delete colorDialog;
}

void ConfigGUI::onForegroundColorChanged(MColorDialog* colorDialog)
{
    // Example name is "chatColorDialog". Expected element name is "chat".
    // Removing "ColorDialog".
    QString element = colorDialog->objectName().left(colorDialog->objectName().length() - 11);
    emit colorChanged(element + "_foregroundcolor", colorDialog->currentColor());
}