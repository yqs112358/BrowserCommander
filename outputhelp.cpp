#include "outputhelp.h"
#include <cstdio>

OutputHelp::OutputHelp()
{

}

bool OutputHelp::changeOutput(QString outputFile)
{
    if(outDevice.isOpen())
    {
        outDevice.flush();
        outDevice.close();
    }
    if(stdOut != nullptr)
        delete stdOut;

    if(outputFile.isEmpty())
        stdOut=new QTextStream(stdout,QIODevice::WriteOnly|QIODevice::Text);
    else
    {
        outDevice.setFileName(outputFile);
        if(!outDevice.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            stdOut=new QTextStream(stdout,QIODevice::WriteOnly|QIODevice::Text);
            return false;
        }
        else
            stdOut=new QTextStream(&outDevice);
    }
    return true;
}

QString OutputHelp::dateTimeStr()
{
    return QDateTime::currentDateTime().toString("hh:mm:ss");
}
