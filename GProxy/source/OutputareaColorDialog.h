#ifndef _OUTPUTAREACOLORDIALOG_H
#define	_OUTPUTAREACOLORDIALOG_H

#include "ui_OutputareaColorDialog.h"

class OutputareaColorDialog : public QDialog
{
    Q_OBJECT
public:
    OutputareaColorDialog();
    virtual ~OutputareaColorDialog();
private:
    Ui::OutputareaColorDialog widget;
};

#endif	/* _OUTPUTAREACOLORDIALOG_H */
