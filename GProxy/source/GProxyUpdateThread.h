#ifndef GPROXYUPDATETHREAD_H
#define	GPROXYUPDATETHREAD_H

#include "gproxy.h"
#include "bnet.h"
#include <QThread>

class GProxyUpdateThread  : public QThread {
    Q_OBJECT

private:
    CGProxy *gproxy;

public:
    GProxyUpdateThread(CGProxy *p_gproxy);
    virtual ~GProxyUpdateThread();

protected:
    void run();

};

#endif	/* GPROXYUPDATETHREAD_H */

