#include "GProxyUpdateThread.h"

GProxyUpdateThread::GProxyUpdateThread (CGProxy *p_gproxy)
{
    gproxy = p_gproxy;
}

GProxyUpdateThread::~GProxyUpdateThread () { }

void GProxyUpdateThread::run ()
{
    while (true)
    {
        if (gproxy->Update(40000))
            break;
    }
}