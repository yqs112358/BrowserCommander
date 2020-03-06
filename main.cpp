#include <QApplication>
#include <QTranslator>
#include <QStyleFactory>
#include <QLocale>
#include <QMutex>
#include <QCommandLineParser>
#include <QSettings>
#include <QFile>

#include <windows.h>
#include <cstdio>

#include "commander.h"
#include "outputhelp.h"

#define VER L"0.6.0"

extern OutputHelp output;

void outputTransfer(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
        Q_UNUSED(context)

        static QMutex mutex;
        mutex.lock();
        switch(type)
        {
        case QtDebugMsg:
            *(output.stdOut) << msg;
            output.stdOut->flush();
            break;
        case QtInfoMsg:
            if(output.debugOutDevice.isOpen())
            {
                output.debugOut << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                << " Info]" << msg;
                output.debugOut.flush();
            }
            break;
        case QtWarningMsg:
            if(output.debugOutDevice.isOpen())
            {
                output.debugOut << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                << " Warning]" << msg;
                output.debugOut.flush();
            }
            break;
        case QtCriticalMsg:
            if(output.debugOutDevice.isOpen())
            {
                output.debugOut << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                << " Critical]" << msg;
                output.debugOut.flush();
            }
            break;
        case QtFatalMsg:
            if(output.debugOutDevice.isOpen())
            {
                output.debugOut << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
                                << " FATAL]" << msg;
                output.debugOut.flush();
            }
            break;
        }

        mutex.unlock();
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);
    //Settings
    a.setWindowIcon(QIcon(QStringLiteral(":AppLogoColor.png")));
    QCoreApplication::setOrganizationName("BrowserCommander");
    QCoreApplication::setApplicationName("BrowserCommander");
    QCoreApplication::setApplicationVersion(QString::fromWCharArray(VER));
    //qputenv("QTWEBENGINEPROCESS_PATH", QStringLiteral("./BrowserCommanderWeb.exe").toLocal8Bit());

    //Command-line analysis
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription(QObject::tr("A simple browser which can be controlled by CLI and script files"));
    parser.addPositionalArgument(".bcs script file",".bcs script file which is runned automatically");
    parser.process(a);
    QStringList autoScripts(parser.positionalArguments());

    //Translator
    QTranslator *trans=new QTranslator;
    QSettings conf(CONF_PATH);
    QString lang=conf.value("Language","auto").toString();
    QString transFileName=QString("translations/BrowserCommander_%1.qm").arg(lang);
    if((lang == "auto" || !QFile::exists(transFileName)) || !trans->load(transFileName))
    {
        QLocale locale;
        if(locale.language() == QLocale::Chinese)
        {
            trans->load("translations/BrowserCommander_zh_CN.qm");
            a.installTranslator(trans);
        }
    }
    else
        a.installTranslator(trans);


    //Style
    a.setStyle(QStyleFactory::create("fusion"));

    //Create New Console
    //ONLY WINDOWS!
    AllocConsole();
    FILE *tmp,*tmp2;
    freopen_s(&tmp,"CONIN$","r",stdin);
    freopen_s(&tmp2,"CONOUT$","w",stdout);
    wprintf(QObject::tr("[---BrowserCommander---] Version %s   Author:yqs112358\n[License] MIT License\n"
                        "[Project on GitHub] https://github.com/yqs112358/BrowserCommander\n"
                        "Welcome to your new ideas!\n").toStdWString().c_str(),VER);

    //Install output
    output.changeOutput(QString());
    qInstallMessageHandler(outputTransfer);

    Commander commander(autoScripts);

    return a.exec();
}
