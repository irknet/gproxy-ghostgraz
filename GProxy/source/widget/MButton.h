#ifndef MBUTTON_H
#define	MBUTTON_H

#include <QPushButton>

class MButton : public QPushButton
{
    Q_OBJECT

public:
    MButton(QWidget* parent = 0);
    MButton(QString text, QWidget* parent = 0);
    virtual ~MButton();

signals:
    void clicked(MButton* button);

private slots:
    void onButtonClicked();
};

#endif	/* MBUTTON_H */

