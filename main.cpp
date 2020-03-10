#pragma execution_character_set("utf-8")

#include <QApplication>
#include <QTranslator>
#include <QStyleFactory>
#include <QLocale>
#include <QMutex>
#include <QCommandLineParser>
#include <QSettings>
#include <QFile>
#include <QByteArray>
#include <QTextCodec>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#define _S(text) QStringLiteral(text)
#define APP_NAME "BrowserCommander"
#define VER "0.6.1"

#include "commander.h"
#include "iohelp.h"

extern IoHelp iohelp;

void outputTransfer(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context)

    static QMutex mutex;
    QString foreOutput;
    if(type != QtDebugMsg)
    {
        foreOutput=_S("[%1 ")
                .arg(QDateTime::currentDateTime().toString(_S("yyyy-MM-dd hh:mm:ss")));
        switch(type)
        {
        case QtInfoMsg:
            foreOutput.append(_S("Info]"));
            break;
        case QtWarningMsg:
            foreOutput.append(_S("Warning]"));
            break;
        case QtCriticalMsg:
            foreOutput.append(_S("Critical]"));
            break;
        case QtFatalMsg:
            foreOutput.append(_S("FATAL]"));
            break;
        default:
            break;
        }
    }
    mutex.lock();
    if(type == QtDebugMsg)
    {
        iohelp.cout << msg;
    }
    else
    {
        if(iohelp.cerrDevice.isOpen())
            iohelp.cerr << foreOutput << msg;
    }
    iohelp.cout.flush();
    mutex.unlock();
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);
    //Settings
    a.setWindowIcon(QIcon(_S(":AppLogoColor.png")));
    QCoreApplication::setOrganizationName(_S(APP_NAME));
    QCoreApplication::setApplicationName(_S(APP_NAME));
    QCoreApplication::setApplicationVersion(_S(VER));
    //Win settings
    #ifdef Q_OS_WIN
    //QCoreApplication::addLibraryPath();
    qputenv("QTWEBENGINEPROCESS_PATH", _S("./BrowserCommanderWeb.exe").toLocal8Bit());
    SetConsoleTitleA(APP_NAME);
    #endif

    //Command-line analysis
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription(QObject::tr("A simple browser which can be controlled by CLI and script files"));
    parser.addPositionalArgument(_S(".bcs script file")
                                 ,QObject::tr(".bcs script file which is runned automatically"));
    parser.process(a);
    QStringList autoScripts(parser.positionalArguments());

    //Translator
    QTranslator *trans=new QTranslator;
    QSettings conf(_S(CONF_PATH),QSettings::IniFormat);
    conf.beginGroup(_S("Main"));
    QString lang=conf.value(_S("Language"),_S("auto")).toString();
    if(lang != _S("en_US"))
    {
        QString transFileName=_S(":BrowserCommander_%1.qm").arg(lang);
        if((lang == _S("auto") || !QFile::exists(transFileName)) || !trans->load(transFileName))
        {
            QLocale locale;
            if(locale.language() == QLocale::Chinese && trans->load(_S(":BrowserCommander_zh_CN.qm")))
                a.installTranslator(trans);
        }
        else
            a.installTranslator(trans);
    }
    conf.endGroup();

    //Style
    a.setStyle(QStyleFactory::create(_S("fusion")));

    //Install output
    iohelp.changeOutput(QString());
    qInstallMessageHandler(outputTransfer);
    qDebug() << qPrintable(QObject::tr("<--- BrowserCommander ---> Version %1   Author:yqs112358\n"
                            "[License] BSD-2-Clause\n"
                            "[Project on GitHub] https://github.com/yqs112358/BrowserCommander\n"
                            "[Project on GitHub] Welcome to your new ideas!\n"
                            "Tips: Use the command 'Help' to get guidance.\n"
                            ).arg(_S(VER)));

    Commander commander(autoScripts);

    return a.exec();
}
