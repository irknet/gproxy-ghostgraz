#include "ConfigGUI.h"
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QToolTip>
#include <QPoint>
#include <QMessageBox>
#include <QIcon>

ConfigGUI::ConfigGUI (Config *cfg)
{
    widget.setupUi(this);
    this->cfg = cfg;
    this->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint
            | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    init();
}

ConfigGUI::~ConfigGUI () { }

void ConfigGUI::init ()
{
    this->setFixedSize(450, 600);
    widget.buttonBox->setGeometry(0, this->height() - 30, this->width(), 30);

    int currentHeight = 5;
    QFont textfieldFont("Arial", 9);

    smTextfieldTextChanged = new QSignalMapper(this);
    smServer = new QSignalMapper(this);
    smFileDialog = new QSignalMapper(this);

    QVector<QString> vKey = cfg->getKeys();
    QVector<QString> vValue = cfg->getValues();

    for (int i = 0; i < vKey.count(); i++)
    {
        QString key = vKey.at(i);
        QString value = vValue.at(i);

        QLabel *label = new QLabel(key, this);
        label->setAccessibleName(key);

        if (key.startsWith("#"))
        {
            if (key == "# Application config values")
            {
                delete label;
                break;
            }
            else
            {
                currentHeight += 10;
                label->setText(key.mid(2));
                label->setGeometry(10, currentHeight, this->width(), 25);
                label->setAlignment(Qt::AlignCenter);
                label->setFont(QFont("Arial", 10, QFont::Bold));
                currentHeight += 32;
                continue;
            }
        }
        else
        {
            label->setGeometry(10, currentHeight, 200, 20);
            label->setFont(QFont("Segoe UI", 9, QFont::Bold));

            if (key == "war3path")
            {
                label->setText("Warcraft 3 Install path:");
                FileDialogLineEdit *textfield = new FileDialogLineEdit(value, this);
                textfield->setGeometry(180, currentHeight, 200, 20);
                textfield->setFont(textfieldFont);
                textfield->setAccessibleName(key);
                QPushButton *fileButton = new QPushButton("...", this);
                fileButton->setGeometry(380, currentHeight, 30, 20);
                connect(textfield, SIGNAL(clicked()),
                        smFileDialog, SLOT(map()));
                smFileDialog->setMapping(textfield, textfield);
                connect(fileButton, SIGNAL(clicked()),
                        smFileDialog, SLOT(map()));
                smFileDialog->setMapping(fileButton, textfield);
            }
            else if (key == "cdkeyroc")
            {
                label->setText("Reign of Chaos CD key:");
                QLineEdit *textfield = new QLineEdit(value, this);
                textfield->setGeometry(180, currentHeight, 230, 20);
                textfield->setFont(textfieldFont);
                textfield->setAccessibleName(key);
                connect(textfield, SIGNAL(textChanged(const QString &)),
                        smTextfieldTextChanged, SLOT(map()));
                smTextfieldTextChanged->setMapping(textfield, textfield);
            }
            else if (key == "cdkeytft")
            {
                label->setText("The Frozen Throne CD key:");
                QLineEdit *textfield = new QLineEdit(value, this);
                textfield->setGeometry(180, currentHeight, 230, 20);
                textfield->setFont(textfieldFont);
                textfield->setAccessibleName(key);
                connect(textfield, SIGNAL(textChanged(const QString &)),
                        smTextfieldTextChanged, SLOT(map()));
                smTextfieldTextChanged->setMapping(textfield, textfield);
            }
            else if (key == "server")
            {
                label->setText("Battle.net server:");
                QComboBox *combobox = new QComboBox(this);
                combobox->setGeometry(180, currentHeight, 230, 20);
                combobox->setFont(textfieldFont);
                combobox->setAccessibleName(key);
                combobox->addItem("US West (Lordaeron)");
                combobox->addItem("US East (Azeroth)");
                combobox->addItem("Asia (Kalimdor)");
                combobox->addItem("Europe (Northrend)");
                combobox->addItem("PvPGN Server");
                setSelectedCBValue(combobox, value);

                connect(combobox, SIGNAL(currentIndexChanged(const QString&)),
                        smServer, SLOT(map()));
                smServer->setMapping(combobox, combobox);
            }
            else if (key == "username")
            {
                label->setText("Username:");
                QLineEdit *textfield = new QLineEdit(value, this);
                textfield->setGeometry(180, currentHeight, 230, 20);
                textfield->setFont(textfieldFont);
                textfield->setAccessibleName(key);
                connect(textfield, SIGNAL(textChanged(const QString &)),
                        smTextfieldTextChanged, SLOT(map()));
                smTextfieldTextChanged->setMapping(textfield, textfield);
            }
            else if (key == "password")
            {
                label->setText("Password:");
                QLineEdit *textfield = new QLineEdit(value, this);
                textfield->setGeometry(180, currentHeight, 230, 20);
                textfield->setFont(textfieldFont);
                textfield->setAccessibleName(key);
                textfield->setEchoMode(QLineEdit::Password);
                connect(textfield, SIGNAL(textChanged(const QString &)),
                        smTextfieldTextChanged, SLOT(map()));
                smTextfieldTextChanged->setMapping(textfield, textfield);
            }
            else if (key == "channel")
            {
                label->setText("Channel:");
                QLineEdit *textfield = new QLineEdit(value, this);
                textfield->setGeometry(180, currentHeight, 230, 20);
                textfield->setFont(textfieldFont);
                textfield->setAccessibleName(key);
                connect(textfield, SIGNAL(textChanged(const QString &)),
                        smTextfieldTextChanged, SLOT(map()));
                smTextfieldTextChanged->setMapping(textfield, textfield);
            }
            else if (key == "war3version")
            {
                label->setText("Warcraft 3 patch version:");
                QLineEdit *textfield = new QLineEdit(value, this);
                textfield->setGeometry(180, currentHeight, 230, 20);
                textfield->setFont(textfieldFont);
                textfield->setAccessibleName(key);
                QIntValidator *validator = new QIntValidator(this);
                validator->setBottom(0);
                textfield->setValidator(validator);
                connect(textfield, SIGNAL(textChanged(const QString &)),
                        smTextfieldTextChanged, SLOT(map()));
                smTextfieldTextChanged->setMapping(textfield, textfield);
            }
            else if (key == "port")
            {
                label->setText("Port:");
                QLineEdit *textfield = new QLineEdit(value, this);
                textfield->setGeometry(180, currentHeight, 230, 20);
                textfield->setFont(textfieldFont);
                textfield->setAccessibleName(key);
                QIntValidator *validator = new QIntValidator(this);
                validator->setBottom(1024);
                textfield->setValidator(validator);
                connect(textfield, SIGNAL(textChanged(const QString &)),
                        smTextfieldTextChanged, SLOT(map()));
                smTextfieldTextChanged->setMapping(textfield, textfield);
            }
            else if (key == "exeversion")
            {
                label->setText("Exe version:");
                QLineEdit *textfield = new QLineEdit(value, this);
                textfield->setGeometry(180, currentHeight, 230, 20);
                textfield->setFont(textfieldFont);
                textfield->setAccessibleName(key);
                connect(textfield, SIGNAL(textChanged(const QString &)),
                        smTextfieldTextChanged, SLOT(map()));
                smTextfieldTextChanged->setMapping(textfield, textfield);
            }
            else if (key == "exeversionhash")
            {
                label->setText("Exe version hash:");
                QLineEdit *textfield = new QLineEdit(value, this);
                textfield->setGeometry(180, currentHeight, 230, 20);
                textfield->setFont(textfieldFont);
                textfield->setAccessibleName(key);
                connect(textfield, SIGNAL(textChanged(const QString &)),
                        smTextfieldTextChanged, SLOT(map()));
                smTextfieldTextChanged->setMapping(textfield, textfield);
            }
            else if (key == "passwordhashtype")
            {
                label->setText("Password hash type:");
                QLineEdit *textfield = new QLineEdit(value, this);
                textfield->setGeometry(180, currentHeight, 230, 20);
                textfield->setFont(textfieldFont);
                textfield->setAccessibleName(key);
                connect(textfield, SIGNAL(textChanged(const QString &)),
                        smTextfieldTextChanged, SLOT(map()));
                smTextfieldTextChanged->setMapping(textfield, textfield);
            }
            else if (key == "sound")
            {
                label->setText("Sound:");
                QComboBox *combobox = new QComboBox(this);
                combobox->setGeometry(180, currentHeight, 230, 20);
                combobox->setFont(textfieldFont);
                combobox->setAccessibleName(key);
                combobox->addItem("On");
                combobox->addItem("Off");
                if (cfg->getBoolean(key) == false)
                {
                    combobox->setCurrentIndex(1);
                }
            }
            else if (key == "privategamename")
            {
                label->setText("Privategamename:");
                QLineEdit *textfield = new QLineEdit(value, this);
                textfield->setGeometry(180, currentHeight, 230, 20);
                textfield->setFont(textfieldFont);
                textfield->setAccessibleName(key);
                connect(textfield, SIGNAL(textChanged(const QString &)),
                        smTextfieldTextChanged, SLOT(map()));
                smTextfieldTextChanged->setMapping(textfield, textfield);
            }
            else if (key == "botprefix")
            {
                label->setText("Botprefix:");
                QLineEdit *textfield = new QLineEdit(value, this);
                textfield->setGeometry(180, currentHeight, 230, 20);
                textfield->setFont(textfieldFont);
                textfield->setAccessibleName(key);
                connect(textfield, SIGNAL(textChanged(const QString &)),
                        smTextfieldTextChanged, SLOT(map()));
                smTextfieldTextChanged->setMapping(textfield, textfield);
            }
            else if (key == "autosearch")
            {
                label->setText("Autosearch:");
                QComboBox *combobox = new QComboBox(this);
                combobox->setGeometry(180, currentHeight, 230, 20);
                combobox->setFont(textfieldFont);
                combobox->setAccessibleName(key);
                combobox->addItem("On");
                combobox->addItem("Off");
                if (cfg->getBoolean(key) == false)
                {
                    combobox->setCurrentIndex(1);
                }
            }
            else if (key == "log")
            {
                label->setText("Enable logging:");
                QComboBox *combobox = new QComboBox(this);
                combobox->setGeometry(180, currentHeight, 230, 20);
                combobox->setFont(textfieldFont);
                combobox->setAccessibleName(key);
                combobox->addItem("True");
                combobox->addItem("False");
                if (cfg->getBoolean(key) == false)
                {
                    combobox->setCurrentIndex(1);
                }
            }
            else
            {
                QLineEdit *textfield = new QLineEdit(value, this);
                textfield->setGeometry(180, currentHeight, 230, 20);
                textfield->setFont(textfieldFont);
                textfield->setAccessibleName(key);
                connect(textfield, SIGNAL(textChanged(const QString &)),
                        smTextfieldTextChanged, SLOT(map()));
                smTextfieldTextChanged->setMapping(textfield, textfield);
            }
        }

        currentHeight += 22;
    }

    connect(smTextfieldTextChanged, SIGNAL(mapped(QWidget *)),
            this, SLOT(onTextfieldTextChanged(QWidget *)));

    connect(smServer, SIGNAL(mapped(QWidget *)),
            this, SLOT(onServerCBChanged(QWidget *)));

    connect(smFileDialog, SIGNAL(mapped(QWidget *)),
            this, SLOT(onOpenFileDialog(QWidget *)));
}

void ConfigGUI::accept ()
{
    QList<QObject*> child = this->children();
    for (int i = 0; i < child.count(); i++)
    {
        if (qobject_cast<QLineEdit*> (child.at(i)) != NULL)
        {
            QLineEdit *tf = (QLineEdit*) child.at(i);
            cfg->setString(tf->accessibleName(), tf->text());
        }
        else if(qobject_cast<QComboBox*> (child.at(i)) != NULL)
        {
            QComboBox *cb = (QComboBox*) child.at(i);
            if (cb->accessibleName() == "server")
            {
                if (cb->currentText() == "US West (Lordaeron)")
                {
                    cfg->setString("server", "uswest.battle.net");
                }
                else if (cb->currentText() == "US East (Azeroth)")
                {
                    cfg->setString("server", "useast.battle.net");
                }
                else if (cb->currentText() == "Asia (Kalimdor)")
                {
                    cfg->setString("server", "asia.battle.net");
                }
                else if (cb->currentText() == "Europe (Northrend)")
                {
                    cfg->setString("server", "europe.battle.net");
                }
                else
                {
                    cfg->setString("server", cb->currentText());
                }
            }
            else
            {
                cfg->setString(cb->accessibleName(), cb->currentText());
            }
        }
    }

    if(cfg->hasRequiredValues())
    {
        cfg->commit();
        done(QDialog::Accepted);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowIcon(QIcon(":/images/images/Error.png"));
        msgBox.setWindowTitle("Error");
        msgBox.setText("You need to fill in all required values to save!");
        msgBox.exec();
    }
}

void ConfigGUI::reject ()
{
    cfg->loadConfig();
    done(QDialog::Rejected);
}

void ConfigGUI::onServerCBChanged (QWidget *combobox)
{
    QComboBox *cb = (QComboBox*) combobox;

    if (cb->currentText() == "PvPGN Server")
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
            setSelectedCBValue(cb, textfield->text());
        }
        else
        {
            setSelectedCBValue(cb, cb->accessibleDescription());
        }
    }
    else
    {
        cb->setAccessibleDescription(cb->currentText());
    }
}

void ConfigGUI::setSelectedCBValue (QComboBox *combobox, QString value)
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
        combobox->addItem(value);
        combobox->setCurrentIndex(combobox->count() - 1);
    }

    combobox->setAccessibleDescription(combobox->currentText());
}

void ConfigGUI::onTextfieldTextChanged (QWidget * textfield)
{
    QLineEdit *tf = (QLineEdit*) textfield;
    QString key = tf->accessibleName();
    QString value = tf->text();
    QPoint position(tf->pos().x() + this->x() + tf->width() + 7,
            this->y() + tf->pos().y() + 10);
    int cursorPosition = tf->cursorPosition();

    if (key == "cdkeyroc" || key == "cdkeytft")
    {
        tf->setText(value.toUpper());
        tf->setCursorPosition(cursorPosition);

        if (value.length() < 26)
        {
            QPalette pal;
            pal.setColor(QPalette::Text, Qt::red);
            tf->setPalette(pal);
            QToolTip::showText(position,
                    "The cd key has to be 26 characters long", tf);
        }
        else if (value.length() > 26)
        {
            tf->setText(value.remove(cursorPosition - 1, 1));
            tf->setCursorPosition(cursorPosition - 1);
            QPalette pal;
            pal.setColor(QPalette::Text, Qt::black);
            tf->setPalette(pal);
            QToolTip::showText(position,
                    "The cd key has to be 26 characters long", tf);
        }
        else
        {
            QPalette pal;
            pal.setColor(QPalette::Text, Qt::black);
            tf->setPalette(pal);
        }

        if (value.contains("-"))
        {
            tf->setText(value.replace("-", ""));
            QToolTip::showText(position,
                    "Enter the cd key without dashes", tf);
        }
    }
    else if (key == "username" || key == "password")
    {
        if (value.length() < 3)
        {
            QPalette pal;
            pal.setColor(QPalette::Text, Qt::red);
            tf->setPalette(pal);
            QToolTip::showText(position,
                    "The " + key + " has be at least 3 characters long", tf);
        }
        else if (value.length() > 15)
        {
            tf->setText(value.remove(cursorPosition - 1, 1));
            tf->setCursorPosition(cursorPosition - 1);
            QPalette pal;
            pal.setColor(QPalette::Text, Qt::black);
            tf->setPalette(pal);
            QToolTip::showText(position,
                    "The " + key + " has a maximum length of 15 characters", tf);
        }
        else
        {
            QPalette pal;
            pal.setColor(QPalette::Text, Qt::black);
            tf->setPalette(pal);
        }
    }
    else if (key == "channel")
    {
        if (value.length() > 31)
        {
            tf->setText(value.remove(cursorPosition - 1, 1));
            tf->setCursorPosition(cursorPosition - 1);
            QToolTip::showText(position,
                    "The channel name has a maximum length of 31 characters", tf);
        }
    }
    else if (key == "log")
    {
        if (!value.endsWith(".txt"))
        {
            QToolTip::showText(position,
                    "It is recommended that the log file is a textfile (.txt)", tf);
        }
    }
}

void ConfigGUI::onOpenFileDialog (QWidget* textfield)
{
    QLineEdit *tf = (QLineEdit*) textfield;
    QFileDialog fileDialog(this);
    fileDialog.setWindowTitle("Choose your Warcraft III Install directory");
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    //    fileDialog.setOption(QFileDialog::ShowDirsOnly);

    if (fileDialog.exec() == QDialog::Accepted)
    {
        tf->setText(QDir::toNativeSeparators(fileDialog.selectedFiles().at(0))
                + QDir::separator());
    }
}