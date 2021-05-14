#include "enginehelper.h"
#include <QDateTime>
#include <QTimer>

#define _S(text) QStringLiteral(text)

EngineHelper::EngineHelper(Commander *c) : commander(c)
{

}

void EngineHelper::waitLoading(int maxTime, int checkInterval)
{
    QEventLoop eventloop;
    while(isLoading)
    {
        QTimer::singleShot(checkInterval, &eventloop, SLOT(quit()));
        eventloop.exec();
        if(maxTime >= 0 && (maxTime-=checkInterval)<=0)
            break;
    }
    isLoading=false;
}

void EngineHelper::sFinished()
{
    commander->isWaitingFinished=false;
}

void EngineHelper::sLoadStart()
{
    isLoading=true;
}

void EngineHelper::sLoadEnd()
{
    isLoading=false;
}
