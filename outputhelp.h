#ifndef OUTPUTHELP_H
#define OUTPUTHELP_H

#include <QTextStream>
#include <QFile>
#include <QMutex>
#include <QDateTime>
#include <QString>

class OutputHelp
{
public:
    QFile outDevice;
    QFile debugOutDevice;
    QTextStream *stdOut;
    QTextStream debugOut;

    OutputHelp();
    bool changeOutput(QString outputFile);
    static QString dateTimeStr();
    //static void outputTransfer(QtMsgType type,const QMessageLogContext &context,const QString &msg);
};

static OutputHelp output;

#endif // OUTPUTHELP_H
