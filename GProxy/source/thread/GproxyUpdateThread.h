#ifndef GPROXYUPDATETHREAD_H
#define	GPROXYUPDATETHREAD_H

#include "gproxy.h"

#include <QThread>

class GproxyUpdateThread  : public QThread {
    Q_OBJECT

private:
    CGProxy *gproxy;
    bool stopped;

public:
    GproxyUpdateThread(CGProxy *p_gproxy);
    virtual ~GproxyUpdateThread();
    void stop();

protected:
    void run();

};

#endif	/* GPROXYUPDATETHREAD_H */

