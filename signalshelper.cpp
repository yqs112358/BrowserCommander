#include "signalshelper.h"
#include <QDateTime>

#define _S(text) QStringLiteral(text)

SignalsHelper::SignalsHelper(Commander *c) : commander(c)
{

}

void SignalsHelper::waitLoading(int maxTime, int checkInterval)
{
    QCoreApplication::processEvents();
    while(isLoading || (maxTime >=0 && (maxTime-=checkInterval)>0))
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
