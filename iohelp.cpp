#include "iohelp.h"
#include <cstdio>

#define _S(text) QStringLiteral(text)

IoHelp iohelp;

IoHelp::IoHelp()
    :cout(&coutDevice),cerr(&cerrDevice)
{
    cout.setAutoDetectUnicode(true);
    cerr.setAutoDetectUnicode(true);
}

bool IoHelp::changeOutput(QString outputFile)
{
    if(coutDevice.isOpen())
    {
        coutDevice.flush();
        coutDevice.close();
    }

    if(outputFile.isEmpty())
    {
        coutDevice.open(stdout,QIODevice::WriteOnly|QIODevice::Text);
    }
    else
    {
        coutDevice.setFileName(outputFile);
        if(!coutDevice.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            coutDevice.open(stdout,QIODevice::WriteOnly|QIODevice::Text);
            return false;
        }
    }
    return true;
}

QString IoHelp::dateTimeStr()
{
    return QDateTime::currentDateTime().toString(_S("hh:mm:ss"));
}
