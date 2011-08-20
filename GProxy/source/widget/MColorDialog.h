#ifndef MCOLORDIALOG_H
#define	MCOLORDIALOG_H

#include <QColorDialog>

class MColorDialog : public QColorDialog
{
    Q_OBJECT

public:
    MColorDialog(QWidget* parent = 0);
    MColorDialog(QColor startColor, QWidget* parent = 0);
    virtual ~MColorDialog();

signals:
    void currentColorChanged(MColorDialog* colorDialog);

private slots:
    void onCurrentColorChanged(const QColor&);
};

#endif	/* MCOLORDIALOG_H */

