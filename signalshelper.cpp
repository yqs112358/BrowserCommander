#include "signalshelper.h"
#include <QDateTime>

SignalsHelper::SignalsHelper(Commander *c) : commander(c)
{

}

void SignalsHelper::waitLoading(int checkInterval)
{
    QCoreApplication::processEvents();
    while(isLoading)
    {
        QThread::msleep(checkInterval);
        QCoreApplication::processEvents();
    }
    isLoading=false;
}

void SignalsHelper::sFinished()
{
    commander->isWaitingFinished=false;
}

void SignalsHelper::sLoadStart()
{
    isLoading=true;
}

void SignalsHelper::sLoadEnd()
{
    isLoading=false;
}
