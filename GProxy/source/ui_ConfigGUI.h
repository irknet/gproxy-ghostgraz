/********************************************************************************
** Form generated from reading UI file 'ConfigGUI.ui'
**
** Created: Sat 21. May 15:55:00 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGGUI_H
#define UI_CONFIGGUI_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfigGUI
{
public:
    QDialogButtonBox *buttonBox;
    QTabWidget *tabWidget;
    QWidget *connectionTab;
    QLabel *channelLabel;
    QLabel *serverLabel;
    QLabel *tftKeyLabel;
    QLabel *war3versionLabel;
    QLabel *usernameLabel;
    QLabel *war3pathLabel;
    QLabel *rocKeyLabel;
    QLabel *passwordLabel;
    QLabel *portLabel;
    QLabel *connectionHeaderLabel;
    QLineEdit *cdkeyrocTextfield;
    QLineEdit *cdkeytftTextfield;
    QLineEdit *usernameTextfield;
    QLineEdit *passwordTextfield;
    QLineEdit *channelTextfield;
    QLineEdit *war3versionTextfield;
    QLineEdit *portTextfield;
    QComboBox *serverCombobox;
    QPushButton *war3pathButton;
    QWidget *pvpgnTab;
    QLabel *pvpgnHeaderLabel;
    QLabel *exeversionhashLabel;
    QLineEdit *exeversionhashTextfield;
    QLabel *exeversionLabel;
    QLineEdit *exeversionTextfield;
    QLabel *passwordhashtypeLabel;
    QLineEdit *passwordhashtypeTextfield;
    QWidget *optionalTab;
    QLabel *optionalSettingsHeaderLabel;
    QLabel *soundLabel;
    QLabel *privategamenameLabel;
    QLabel *botprefixLabel;
    QLabel *autosearchLabel;
    QLabel *loggingLabel;
    QComboBox *soundCombobox;
    QLineEdit *privategamenameTextfield;
    QLineEdit *botprefixTextfield;
    QComboBox *autosearchCombobox;
    QComboBox *logCombobox;

    void setupUi(QDialog *ConfigGUI)
    {
        if (ConfigGUI->objectName().isEmpty())
            ConfigGUI->setObjectName(QString::fromUtf8("ConfigGUI"));
        ConfigGUI->setWindowModality(Qt::ApplicationModal);
        ConfigGUI->resize(450, 550);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ConfigGUI->sizePolicy().hasHeightForWidth());
        ConfigGUI->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/Tool.png"), QSize(), QIcon::Normal, QIcon::Off);
        ConfigGUI->setWindowIcon(icon);
        buttonBox = new QDialogButtonBox(ConfigGUI);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(0, 520, 450, 30));
        buttonBox->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);
        tabWidget = new QTabWidget(ConfigGUI);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 450, 520));
        connectionTab = new QWidget();
        connectionTab->setObjectName(QString::fromUtf8("connectionTab"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(9);
        font.setBold(true);
        font.setWeight(75);
        connectionTab->setFont(font);
        channelLabel = new QLabel(connectionTab);
        channelLabel->setObjectName(QString::fromUtf8("channelLabel"));
        channelLabel->setGeometry(QRect(10, 300, 160, 20));
        channelLabel->setFont(font);
        serverLabel = new QLabel(connectionTab);
        serverLabel->setObjectName(QString::fromUtf8("serverLabel"));
        serverLabel->setGeometry(QRect(10, 180, 160, 20));
        serverLabel->setFont(font);
        tftKeyLabel = new QLabel(connectionTab);
        tftKeyLabel->setObjectName(QString::fromUtf8("tftKeyLabel"));
        tftKeyLabel->setGeometry(QRect(10, 140, 160, 20));
        tftKeyLabel->setFont(font);
        war3versionLabel = new QLabel(connectionTab);
        war3versionLabel->setObjectName(QString::fromUtf8("war3versionLabel"));
        war3versionLabel->setGeometry(QRect(10, 340, 160, 20));
        war3versionLabel->setFont(font);
        usernameLabel = new QLabel(connectionTab);
        usernameLabel->setObjectName(QString::fromUtf8("usernameLabel"));
        usernameLabel->setGeometry(QRect(10, 220, 160, 20));
        usernameLabel->setFont(font);
        war3pathLabel = new QLabel(connectionTab);
        war3pathLabel->setObjectName(QString::fromUtf8("war3pathLabel"));
        war3pathLabel->setGeometry(QRect(10, 60, 160, 20));
        war3pathLabel->setFont(font);
        rocKeyLabel = new QLabel(connectionTab);
        rocKeyLabel->setObjectName(QString::fromUtf8("rocKeyLabel"));
        rocKeyLabel->setGeometry(QRect(10, 100, 160, 20));
        rocKeyLabel->setFont(font);
        passwordLabel = new QLabel(connectionTab);
        passwordLabel->setObjectName(QString::fromUtf8("passwordLabel"));
        passwordLabel->setGeometry(QRect(10, 260, 160, 20));
        passwordLabel->setFont(font);
        portLabel = new QLabel(connectionTab);
        portLabel->setObjectName(QString::fromUtf8("portLabel"));
        portLabel->setGeometry(QRect(10, 380, 160, 20));
        portLabel->setFont(font);
        connectionHeaderLabel = new QLabel(connectionTab);
        connectionHeaderLabel->setObjectName(QString::fromUtf8("connectionHeaderLabel"));
        connectionHeaderLabel->setGeometry(QRect(0, 0, 450, 40));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Calibri"));
        font1.setPointSize(16);
        connectionHeaderLabel->setFont(font1);
        connectionHeaderLabel->setAlignment(Qt::AlignCenter);
        cdkeyrocTextfield = new QLineEdit(connectionTab);
        cdkeyrocTextfield->setObjectName(QString::fromUtf8("cdkeyrocTextfield"));
        cdkeyrocTextfield->setGeometry(QRect(180, 100, 240, 20));
        QFont font2;
        font2.setBold(false);
        font2.setWeight(50);
        cdkeyrocTextfield->setFont(font2);
        cdkeytftTextfield = new QLineEdit(connectionTab);
        cdkeytftTextfield->setObjectName(QString::fromUtf8("cdkeytftTextfield"));
        cdkeytftTextfield->setGeometry(QRect(180, 140, 240, 20));
        cdkeytftTextfield->setFont(font2);
        usernameTextfield = new QLineEdit(connectionTab);
        usernameTextfield->setObjectName(QString::fromUtf8("usernameTextfield"));
        usernameTextfield->setGeometry(QRect(180, 220, 240, 20));
        usernameTextfield->setFont(font2);
        passwordTextfield = new QLineEdit(connectionTab);
        passwordTextfield->setObjectName(QString::fromUtf8("passwordTextfield"));
        passwordTextfield->setGeometry(QRect(180, 260, 240, 20));
        passwordTextfield->setFont(font2);
        passwordTextfield->setEchoMode(QLineEdit::Password);
        channelTextfield = new QLineEdit(connectionTab);
        channelTextfield->setObjectName(QString::fromUtf8("channelTextfield"));
        channelTextfield->setGeometry(QRect(180, 300, 240, 20));
        channelTextfield->setFont(font2);
        war3versionTextfield = new QLineEdit(connectionTab);
        war3versionTextfield->setObjectName(QString::fromUtf8("war3versionTextfield"));
        war3versionTextfield->setGeometry(QRect(180, 340, 240, 20));
        war3versionTextfield->setFont(font2);
        portTextfield = new QLineEdit(connectionTab);
        portTextfield->setObjectName(QString::fromUtf8("portTextfield"));
        portTextfield->setGeometry(QRect(180, 380, 240, 20));
        portTextfield->setFont(font2);
        serverCombobox = new QComboBox(connectionTab);
        serverCombobox->setObjectName(QString::fromUtf8("serverCombobox"));
        serverCombobox->setGeometry(QRect(180, 180, 240, 20));
        serverCombobox->setFont(font2);
        serverCombobox->setMaxCount(6);
        war3pathButton = new QPushButton(connectionTab);
        war3pathButton->setObjectName(QString::fromUtf8("war3pathButton"));
        war3pathButton->setGeometry(QRect(390, 60, 30, 20));
        tabWidget->addTab(connectionTab, QString());
        pvpgnTab = new QWidget();
        pvpgnTab->setObjectName(QString::fromUtf8("pvpgnTab"));
        pvpgnHeaderLabel = new QLabel(pvpgnTab);
        pvpgnHeaderLabel->setObjectName(QString::fromUtf8("pvpgnHeaderLabel"));
        pvpgnHeaderLabel->setGeometry(QRect(0, 0, 450, 40));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pvpgnHeaderLabel->sizePolicy().hasHeightForWidth());
        pvpgnHeaderLabel->setSizePolicy(sizePolicy1);
        QFont font3;
        font3.setFamily(QString::fromUtf8("Calibri"));
        font3.setPointSize(16);
        font3.setBold(true);
        font3.setWeight(75);
        pvpgnHeaderLabel->setFont(font3);
        pvpgnHeaderLabel->setAlignment(Qt::AlignCenter);
        exeversionhashLabel = new QLabel(pvpgnTab);
        exeversionhashLabel->setObjectName(QString::fromUtf8("exeversionhashLabel"));
        exeversionhashLabel->setGeometry(QRect(10, 100, 160, 20));
        exeversionhashLabel->setFont(font);
        exeversionhashTextfield = new QLineEdit(pvpgnTab);
        exeversionhashTextfield->setObjectName(QString::fromUtf8("exeversionhashTextfield"));
        exeversionhashTextfield->setGeometry(QRect(180, 100, 240, 20));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Arial"));
        font4.setPointSize(9);
        exeversionhashTextfield->setFont(font4);
        exeversionLabel = new QLabel(pvpgnTab);
        exeversionLabel->setObjectName(QString::fromUtf8("exeversionLabel"));
        exeversionLabel->setGeometry(QRect(10, 60, 160, 20));
        exeversionLabel->setFont(font);
        exeversionTextfield = new QLineEdit(pvpgnTab);
        exeversionTextfield->setObjectName(QString::fromUtf8("exeversionTextfield"));
        exeversionTextfield->setGeometry(QRect(180, 60, 240, 20));
        exeversionTextfield->setFont(font4);
        passwordhashtypeLabel = new QLabel(pvpgnTab);
        passwordhashtypeLabel->setObjectName(QString::fromUtf8("passwordhashtypeLabel"));
        passwordhashtypeLabel->setGeometry(QRect(10, 140, 160, 20));
        passwordhashtypeLabel->setFont(font);
        passwordhashtypeTextfield = new QLineEdit(pvpgnTab);
        passwordhashtypeTextfield->setObjectName(QString::fromUtf8("passwordhashtypeTextfield"));
        passwordhashtypeTextfield->setGeometry(QRect(180, 140, 240, 20));
        passwordhashtypeTextfield->setFont(font4);
        tabWidget->addTab(pvpgnTab, QString());
        optionalTab = new QWidget();
        optionalTab->setObjectName(QString::fromUtf8("optionalTab"));
        optionalSettingsHeaderLabel = new QLabel(optionalTab);
        optionalSettingsHeaderLabel->setObjectName(QString::fromUtf8("optionalSettingsHeaderLabel"));
        optionalSettingsHeaderLabel->setGeometry(QRect(0, 0, 450, 40));
        sizePolicy1.setHeightForWidth(optionalSettingsHeaderLabel->sizePolicy().hasHeightForWidth());
        optionalSettingsHeaderLabel->setSizePolicy(sizePolicy1);
        optionalSettingsHeaderLabel->setFont(font3);
        optionalSettingsHeaderLabel->setAlignment(Qt::AlignCenter);
        soundLabel = new QLabel(optionalTab);
        soundLabel->setObjectName(QString::fromUtf8("soundLabel"));
        soundLabel->setGeometry(QRect(10, 60, 160, 20));
        soundLabel->setFont(font);
        privategamenameLabel = new QLabel(optionalTab);
        privategamenameLabel->setObjectName(QString::fromUtf8("privategamenameLabel"));
        privategamenameLabel->setGeometry(QRect(10, 100, 160, 20));
        privategamenameLabel->setFont(font);
        botprefixLabel = new QLabel(optionalTab);
        botprefixLabel->setObjectName(QString::fromUtf8("botprefixLabel"));
        botprefixLabel->setGeometry(QRect(10, 140, 160, 20));
        botprefixLabel->setFont(font);
        autosearchLabel = new QLabel(optionalTab);
        autosearchLabel->setObjectName(QString::fromUtf8("autosearchLabel"));
        autosearchLabel->setGeometry(QRect(10, 180, 160, 20));
        autosearchLabel->setFont(font);
        loggingLabel = new QLabel(optionalTab);
        loggingLabel->setObjectName(QString::fromUtf8("loggingLabel"));
        loggingLabel->setGeometry(QRect(10, 220, 160, 20));
        loggingLabel->setFont(font);
        soundCombobox = new QComboBox(optionalTab);
        soundCombobox->setObjectName(QString::fromUtf8("soundCombobox"));
        soundCombobox->setGeometry(QRect(180, 60, 240, 20));
        QFont font5;
        font5.setFamily(QString::fromUtf8("Arial"));
        font5.setPointSize(9);
        font5.setBold(false);
        font5.setWeight(50);
        soundCombobox->setFont(font5);
        soundCombobox->setMaxCount(10);
        privategamenameTextfield = new QLineEdit(optionalTab);
        privategamenameTextfield->setObjectName(QString::fromUtf8("privategamenameTextfield"));
        privategamenameTextfield->setGeometry(QRect(180, 100, 240, 20));
        privategamenameTextfield->setFont(font4);
        botprefixTextfield = new QLineEdit(optionalTab);
        botprefixTextfield->setObjectName(QString::fromUtf8("botprefixTextfield"));
        botprefixTextfield->setGeometry(QRect(180, 140, 240, 20));
        botprefixTextfield->setFont(font4);
        autosearchCombobox = new QComboBox(optionalTab);
        autosearchCombobox->setObjectName(QString::fromUtf8("autosearchCombobox"));
        autosearchCombobox->setGeometry(QRect(180, 180, 240, 20));
        autosearchCombobox->setFont(font5);
        autosearchCombobox->setMaxCount(10);
        logCombobox = new QComboBox(optionalTab);
        logCombobox->setObjectName(QString::fromUtf8("logCombobox"));
        logCombobox->setGeometry(QRect(180, 220, 240, 20));
        logCombobox->setFont(font5);
        logCombobox->setMaxCount(10);
        tabWidget->addTab(optionalTab, QString());

        retranslateUi(ConfigGUI);
        QObject::connect(buttonBox, SIGNAL(accepted()), ConfigGUI, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ConfigGUI, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ConfigGUI);
    } // setupUi

    void retranslateUi(QDialog *ConfigGUI)
    {
        ConfigGUI->setWindowTitle(QApplication::translate("ConfigGUI", "GProxy Options", 0, QApplication::UnicodeUTF8));
        channelLabel->setText(QApplication::translate("ConfigGUI", "Channel:", 0, QApplication::UnicodeUTF8));
        serverLabel->setText(QApplication::translate("ConfigGUI", "Battle.net server:", 0, QApplication::UnicodeUTF8));
        tftKeyLabel->setText(QApplication::translate("ConfigGUI", "The Frozen Throne CD key:", 0, QApplication::UnicodeUTF8));
        war3versionLabel->setText(QApplication::translate("ConfigGUI", "Warcraft 3 patch version:", 0, QApplication::UnicodeUTF8));
        usernameLabel->setText(QApplication::translate("ConfigGUI", "Username:", 0, QApplication::UnicodeUTF8));
        war3pathLabel->setText(QApplication::translate("ConfigGUI", "Warcraft 3 Install directory:", 0, QApplication::UnicodeUTF8));
        rocKeyLabel->setText(QApplication::translate("ConfigGUI", "Reign of Chaos CD key:", 0, QApplication::UnicodeUTF8));
        passwordLabel->setText(QApplication::translate("ConfigGUI", "Password:", 0, QApplication::UnicodeUTF8));
        portLabel->setText(QApplication::translate("ConfigGUI", "Port:", 0, QApplication::UnicodeUTF8));
        connectionHeaderLabel->setText(QApplication::translate("ConfigGUI", "Connection", 0, QApplication::UnicodeUTF8));
        cdkeyrocTextfield->setText(QString());
        cdkeyrocTextfield->setPlaceholderText(QApplication::translate("ConfigGUI", "Reign of Chaos CD key", 0, QApplication::UnicodeUTF8));
        cdkeytftTextfield->setPlaceholderText(QApplication::translate("ConfigGUI", "The Frozen Throne CD key", 0, QApplication::UnicodeUTF8));
        usernameTextfield->setPlaceholderText(QApplication::translate("ConfigGUI", "Username", 0, QApplication::UnicodeUTF8));
        passwordTextfield->setText(QString());
        passwordTextfield->setPlaceholderText(QApplication::translate("ConfigGUI", "Password", 0, QApplication::UnicodeUTF8));
        channelTextfield->setPlaceholderText(QApplication::translate("ConfigGUI", "Channel", 0, QApplication::UnicodeUTF8));
        war3versionTextfield->setPlaceholderText(QApplication::translate("ConfigGUI", "Warcraft 3 patch version", 0, QApplication::UnicodeUTF8));
        portTextfield->setPlaceholderText(QApplication::translate("ConfigGUI", "Port", 0, QApplication::UnicodeUTF8));
        serverCombobox->clear();
        serverCombobox->insertItems(0, QStringList()
         << QApplication::translate("ConfigGUI", "US West (Lordaeron)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfigGUI", "US East (Azeroth)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfigGUI", "Asia (Kalimdor)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfigGUI", "Europe (Northrend)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfigGUI", "PvPGN Server", 0, QApplication::UnicodeUTF8)
        );
        war3pathButton->setText(QApplication::translate("ConfigGUI", "...", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(connectionTab), QApplication::translate("ConfigGUI", "Connection", 0, QApplication::UnicodeUTF8));
        pvpgnHeaderLabel->setText(QApplication::translate("ConfigGUI", "Player vs Player Gaming Network", 0, QApplication::UnicodeUTF8));
        exeversionhashLabel->setText(QApplication::translate("ConfigGUI", "Exe vesion hash:", 0, QApplication::UnicodeUTF8));
        exeversionhashTextfield->setText(QString());
        exeversionhashTextfield->setPlaceholderText(QApplication::translate("ConfigGUI", "Exe version hash", 0, QApplication::UnicodeUTF8));
        exeversionLabel->setText(QApplication::translate("ConfigGUI", "Exe version:", 0, QApplication::UnicodeUTF8));
        exeversionTextfield->setText(QString());
        exeversionTextfield->setPlaceholderText(QApplication::translate("ConfigGUI", "Exe version", 0, QApplication::UnicodeUTF8));
        passwordhashtypeLabel->setText(QApplication::translate("ConfigGUI", "Password hash type:", 0, QApplication::UnicodeUTF8));
        passwordhashtypeTextfield->setText(QString());
        passwordhashtypeTextfield->setPlaceholderText(QApplication::translate("ConfigGUI", "Password hash type", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(pvpgnTab), QApplication::translate("ConfigGUI", "PvPGN", 0, QApplication::UnicodeUTF8));
        optionalSettingsHeaderLabel->setText(QApplication::translate("ConfigGUI", "Optional settings", 0, QApplication::UnicodeUTF8));
        soundLabel->setText(QApplication::translate("ConfigGUI", "Sound:", 0, QApplication::UnicodeUTF8));
        privategamenameLabel->setText(QApplication::translate("ConfigGUI", "Privategamename:", 0, QApplication::UnicodeUTF8));
        botprefixLabel->setText(QApplication::translate("ConfigGUI", "Botprefix:", 0, QApplication::UnicodeUTF8));
        autosearchLabel->setText(QApplication::translate("ConfigGUI", "Autosearch:", 0, QApplication::UnicodeUTF8));
        loggingLabel->setText(QApplication::translate("ConfigGUI", "Log:", 0, QApplication::UnicodeUTF8));
        soundCombobox->clear();
        soundCombobox->insertItems(0, QStringList()
         << QApplication::translate("ConfigGUI", "Enabled", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfigGUI", "Disabled", 0, QApplication::UnicodeUTF8)
        );
        privategamenameTextfield->setText(QString());
        privategamenameTextfield->setPlaceholderText(QApplication::translate("ConfigGUI", "Privategamename", 0, QApplication::UnicodeUTF8));
        botprefixTextfield->setText(QString());
        botprefixTextfield->setPlaceholderText(QApplication::translate("ConfigGUI", "Botprefix", 0, QApplication::UnicodeUTF8));
        autosearchCombobox->clear();
        autosearchCombobox->insertItems(0, QStringList()
         << QApplication::translate("ConfigGUI", "Enabled", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfigGUI", "Disabled", 0, QApplication::UnicodeUTF8)
        );
        logCombobox->clear();
        logCombobox->insertItems(0, QStringList()
         << QApplication::translate("ConfigGUI", "Enabled", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfigGUI", "Disabled", 0, QApplication::UnicodeUTF8)
        );
        tabWidget->setTabText(tabWidget->indexOf(optionalTab), QApplication::translate("ConfigGUI", "Optional", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ConfigGUI: public Ui_ConfigGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGGUI_H
