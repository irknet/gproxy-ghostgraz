/********************************************************************************
** Form generated from reading UI file 'ConfigGUI.ui'
**
** Created: Tue 10. May 16:14:43 2011
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
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_ConfigGUI
{
public:
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ConfigGUI)
    {
        if (ConfigGUI->objectName().isEmpty())
            ConfigGUI->setObjectName(QString::fromUtf8("ConfigGUI"));
        ConfigGUI->setWindowModality(Qt::ApplicationModal);
        ConfigGUI->resize(400, 300);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/images/Tool.png"), QSize(), QIcon::Normal, QIcon::Off);
        ConfigGUI->setWindowIcon(icon);
        buttonBox = new QDialogButtonBox(ConfigGUI);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        retranslateUi(ConfigGUI);
        QObject::connect(buttonBox, SIGNAL(accepted()), ConfigGUI, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ConfigGUI, SLOT(reject()));

        QMetaObject::connectSlotsByName(ConfigGUI);
    } // setupUi

    void retranslateUi(QDialog *ConfigGUI)
    {
        ConfigGUI->setWindowTitle(QApplication::translate("ConfigGUI", "GProxy Configuration", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ConfigGUI: public Ui_ConfigGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGGUI_H
