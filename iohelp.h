#ifndef IOHELP_H
#define IOHELP_H

#include <QTextStream>
#include <QFile>
#include <QMutex>
#include <QDateTime>
#include <QString>

class IoHelp
{
public:
    QFile coutDevice;
    QFile cerrDevice;
    QTextStream cout;
    QTextStream cerr;

    IoHelp();
    bool changeOutput(QString outputFile);
    static QString dateTimeStr();
};

#endif // IOHELP_H
