#ifndef DOWNLOADTHREAD_H
#define	DOWNLOADTHREAD_H

#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVector>
#include <QStringList>
#include <QEventLoop>

#include "../gproxy.h"

class DownloadThread : public QThread {
    Q_OBJECT

public:
    DownloadThread(CGProxy* gproxy);
    virtual ~DownloadThread();
    void refresh();
    void stop();

protected:
    void run();

private:
    CGProxy* gproxy;
    bool stopped;
    QUrl url;
    QNetworkAccessManager *manager;

//    QVector<QStringList> sortGamelist(QVector<QStringList> vGamelist);

private slots:
    void downloadFinished(QNetworkReply *reply);

signals:
    void signal_clearGamelist();
    void signal_addGame(QString, QString, QString);
};

#endif	/* DOWNLOADTHREAD_H */

