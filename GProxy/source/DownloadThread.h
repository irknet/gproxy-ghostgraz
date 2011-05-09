#ifndef DOWNLOADTHREAD_H
#define	DOWNLOADTHREAD_H

#include "MainGUI.h"
#include "bnet.h"
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class DownloadThread : public QThread {
    Q_OBJECT

private:
    MainGUI *mainGUI;
    QUrl url;
    QNetworkAccessManager manager;
    QNetworkReply *reply;

public:
    DownloadThread(MainGUI *p_mainGUI);
    virtual ~DownloadThread();

protected:
    void run();

private slots:
    void downloadFinished();
};

#endif	/* DOWNLOADTHREAD_H */

