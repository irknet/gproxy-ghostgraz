/********************************************************************************
** Form generated from reading UI file 'OutputareaColorDialog.ui'
**
** Created: Thu 2. Jun 19:54:56 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OUTPUTAREACOLORDIALOG_H
#define UI_OUTPUTAREACOLORDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_OutputareaColorDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;

    void setupUi(QDialog *OutputareaColorDialog)
    {
        if (OutputareaColorDialog->objectName().isEmpty())
            OutputareaColorDialog->setObjectName(QString::fromUtf8("OutputareaColorDialog"));
        OutputareaColorDialog->resize(502, 594);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(9);
        font.setBold(true);
        font.setWeight(75);
        OutputareaColorDialog->setFont(font);
        buttonBox = new QDialogButtonBox(OutputareaColorDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(90, 530, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);
        label = new QLabel(OutputareaColorDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 211, 51));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Calibri"));
        font1.setPointSize(18);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);
        label_2 = new QLabel(OutputareaColorDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 80, 91, 21));
        label_3 = new QLabel(OutputareaColorDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(30, 120, 91, 21));
        label_4 = new QLabel(OutputareaColorDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(30, 180, 91, 21));
        label_5 = new QLabel(OutputareaColorDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(30, 220, 91, 21));
        label_6 = new QLabel(OutputareaColorDialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(30, 270, 91, 21));
        label_7 = new QLabel(OutputareaColorDialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(20, 320, 91, 21));
        label_8 = new QLabel(OutputareaColorDialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(40, 350, 91, 21));
        label_9 = new QLabel(OutputareaColorDialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(40, 420, 91, 21));

        retranslateUi(OutputareaColorDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), OutputareaColorDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), OutputareaColorDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(OutputareaColorDialog);
    } // setupUi

    void retranslateUi(QDialog *OutputareaColorDialog)
    {
        OutputareaColorDialog->setWindowTitle(QApplication::translate("OutputareaColorDialog", "OutputareaColorDialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("OutputareaColorDialog", "Foregroundcolor", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("OutputareaColorDialog", "Chat", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("OutputareaColorDialog", "Whisper", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("OutputareaColorDialog", "Info", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("OutputareaColorDialog", "TCPSOCKET", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("OutputareaColorDialog", "GPROXY", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("OutputareaColorDialog", "BNET", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("OutputareaColorDialog", "Emote", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("OutputareaColorDialog", "Error", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class OutputareaColorDialog: public Ui_OutputareaColorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OUTPUTAREACOLORDIALOG_H
