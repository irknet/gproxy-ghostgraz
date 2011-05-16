#include "GProxyUpdateThread.h"

GProxyUpdateThread::GProxyUpdateThread (CGProxy *p_gproxy)
{
    gproxy = p_gproxy;
}

GProxyUpdateThread::~GProxyUpdateThread ()
{
    this->exit(0);
}

void GProxyUpdateThread::run ()
{
    while (true)
    {
        if (gproxy->Update(40000))
            break;
    }
}