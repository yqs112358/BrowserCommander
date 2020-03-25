#include "commander.h"
#include <QSettings>
#include <QWebEngineSettings>
#include <QWebEngineScriptCollection>
#include <QWebEngineCookieStore>
#include <QWebEngineHistory>
#include <QtDebug>
#include <QDir>
#include <QDesktopServices>

#define USE_LEFT_ARG(QSTR_NAME) \
    QString QSTR_NAME(cmdIn.readAll().trimmed()); \
    if(QSTR_NAME.isEmpty()) { moreArgs(op); break; }

#define _S(text) QStringLiteral(text)

extern IoHelp iohelp;

Commander::Commander(QStringList autoScripts,QObject *parent)
    :QThread(parent),autorunScripts(autoScripts)
{
    //Connect
    sender=new SignalsHelper(this);
    sender->moveToThread(this);
    connect(sender,&SignalsHelper::go,          this,&Commander::sGo,           Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::back,        this,&Commander::sBack,         Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::forward,     this,&Commander::sForward,      Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::reload,      this,&Commander::sReload,       Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::stop,        this,&Commander::sStop,         Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::restart,     this,&Commander::sRestart,      Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::save,        this,&Commander::sSave,         Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::zoom,        this,&Commander::sZoom,         Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::clearCookie, this,&Commander::sClearCookie,  Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::clearHistory,this,&Commander::sClearHistory, Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::clearCache,  this,&Commander::sClearCache,   Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::history,     this,&Commander::sHistory,      Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::previousTab, this,&Commander::sPreviousTab,  Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::nextTab,     this,&Commander::sNextTab,      Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::newTab,      this,&Commander::sNewTab,       Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::closeTab,    this,&Commander::sCloseTab,     Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::userAgent,   this,&Commander::sUserAgent,    Qt::BlockingQueuedConnection);

    connect(sender,&SignalsHelper::runJs,       this,&Commander::sRunJs,        Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::jsSend,      this,&Commander::sJsSend,       Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::addJs,       this,&Commander::sAddJs,        Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::removeJs,    this,&Commander::sRemoveJs,     Qt::BlockingQueuedConnection);

    connect(sender,&SignalsHelper::restart,     this,&Commander::sRestart,      Qt::BlockingQueuedConnection);
    connect(sender,&SignalsHelper::exit,        this,&Commander::sExit,         Qt::BlockingQueuedConnection);

    connect(this,&Commander::finished,sender,&SignalsHelper::sFinished,Qt::QueuedConnection);

    //Check Directories
    QDir dir(_S("."));
    dir.mkdir(_S("Data"));
    dir.mkdir(_S("AutoRun"));
    dir.mkdir(_S("AutoJs"));

    QSettings conf(_S(CONF_PATH),QSettings::IniFormat);

    //Main Settings
    conf.beginGroup(_S("Main"));
    //Default Output
    QString defaultOutput=conf.value(_S("DefaultOutput"),QString()).toString();
    if(!defaultOutput.isEmpty())
        iohelp.changeOutput(defaultOutput);
    //Debug Output
    QString debugOutput=conf.value(_S("DebugOutput"),QString()).toString();
    if(!debugOutput.isEmpty())
    {
        iohelp.cerrDevice.setFileName(debugOutput);
        iohelp.cerrDevice.open(QIODevice::WriteOnly|QIODevice::Text);
    }
    //CommandEcho
    QString commandEcho=conf.value(_S("CommandEcho"),_S("Auto")).toString();
    if(commandEcho == _S("Auto"))
        cmdEcho=CmdEchoMode::Auto;
    else if(commandEcho == _S("Always"))
        cmdEcho=CmdEchoMode::Always;
    else cmdEcho=CmdEchoMode::No;
    conf.endGroup();

    //Browser Settings
    conf.beginGroup(_S("Browser"));
    //Init Browser
    QWebEngineProfile *defProfile=QWebEngineProfile::defaultProfile();
    defProfile->setCachePath(_S("./Data/"));
    defProfile->setPersistentStoragePath(_S("./Data/"));
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    browser=new Browser;
    browser->createWindow(conf.value(_S("DefaultPrivateMode"),false).toBool());
    changeWindow(0);
    //First Page
    QString firstPage=conf.value(_S("FirstPage"),QString()).toString();
    if(!firstPage.isEmpty())
        tabWidget->setUrl(QUrl::fromUserInput(firstPage));
    //User Agent
    QString ua=conf.value(_S("UserAgent"),QString()).toString();
    if(!ua.isEmpty())
        sUserAgent(ua);

    conf.endGroup();

    //AutoRun Settings
    conf.beginGroup(_S("Main"));
    //AutoRun
    QString autoRunDir=conf.value(_S("AutoRunScriptsDir"),QString()).toString();
    if(!autoRunDir.isEmpty())
    {
        if(autoRunDir.endsWith(_S("/")))
            autoRunDir.remove(autoRunDir.size()-1,1);
        QDir dir(autoRunDir);
        autoRunDir.append(_S("/"));

        QStringList fileList=dir.entryList(QDir::Files);
        QString fileName;
        foreach(fileName,fileList)
            if(fileName.endsWith(_S(".bcs")))
                autorunScripts.append(autoRunDir+fileName);
    }
    //AutoJs
    QString autoAddJs=conf.value(_S("AutoAddJsDir"),QString()).toString();
    if(!autoAddJs.isEmpty())
    {
        if(autoAddJs.endsWith(_S("/")))
            autoAddJs.remove(autoAddJs.size()-1,1);
        QDir dir(autoAddJs);
        autoAddJs.append(_S("/"));

        QStringList fileList=dir.entryList(QDir::Files);
        QString fileName;
        foreach(fileName,fileList)
            if(fileName.endsWith(_S(".js")))
            {
                QFile jsFile(autoRunDir+fileName);
                if(jsFile.open(QIODevice::ReadOnly))
                {
                    QWebEngineScript *script =new QWebEngineScript;
                    script->setName(fileName);
                    script->setSourceCode(jsFile.readAll());
                    script->setWorldId(jsWorld);
                    userScripts.insert(fileName,script);
                    curView->page()->scripts().insert(*script);
                }
            }
    }
    conf.endGroup();

    start();
}

Commander::~Commander()
{

}

void Commander::writeMsg(QString text, Commander::MsgType type)
{
    QString strType;
    switch(type)
    {
        case MsgType::Info:
            strType=_S("Info");
            break;
        case MsgType::Error:
            strType=_S("Error");
            break;
        case MsgType::Warning:
            strType=_S("Warning");
            break;
        case MsgType::Fatal:
            strType=_S("FATAL");
            break;
        default:
            strType=_S("Info");
            break;
    }
    qDebug() << qPrintable(_S("[%1 %2]%3\n").arg(IoHelp::dateTimeStr()).arg(strType).arg(text));
}

void Commander::unknownCmd(QString op)
{
    writeMsg(tr("Unknown Command '%1'.Use the command 'Help' to get more information.").arg(op),MsgType::Error);
}

void Commander::badArgs(QString op)
{
    writeMsg(tr("Bad Argument.Use the command 'Help %1' to get more information.").arg(op),MsgType::Error);
}

void Commander::moreArgs(QString op)
{
    writeMsg(tr("More Argument(s) needed.Use the command 'Help %1' to get more information.").arg(op),MsgType::Error);
}

void Commander::run()
{
    if(startWait > 0)
        QThread::sleep(startWait);
    if(!autorunScripts.isEmpty())
    {
        QString scriptName;
        foreach(scriptName,autorunScripts)
        {
            QFile scriptFile(scriptName);
            if(scriptFile.open(QIODevice::ReadOnly|QIODevice::Text))
            {
                QTextStream fileIn(&scriptFile);
                splitCmds(&fileIn,true);
                scriptFile.close();
            }
        }
        autorunScripts.clear();
    }
    QTextStream stdIn(stdin,QIODevice::ReadOnly|QIODevice::Text);
    splitCmds(&stdIn,false);
}

void Commander::splitCmds(QTextStream *stdIn, bool isScript)
{
    bool tmp=isInScript;
    isInScript=isScript;
    QString cmdLine;
    while(true)
    {
        if(!isInScript)
            qDebug() << qPrintable(_S("\n"));
        qDebug() << qPrintable(_S("%1 %2>").arg(IoHelp::dateTimeStr())
                               .arg(isJsConsoleMode?_S("JsConsole"):_S("Browser")));

        cmdLine=stdIn->readLine();
        if(cmdEcho == CmdEchoMode::Always ||
                (cmdEcho == CmdEchoMode::Auto && isInScript && !cmdLine.startsWith(_S("print "))))
            qDebug() << qPrintable(cmdLine) << "\n";

        bool isEnd=false;
        if(isJsConsoleMode)
        {
            if(cmdLine.toLower() == _S("exit") || cmdLine.toLower() == _S("quit"))
                isJsConsoleMode=false;
            else
            {
                emit sender->runJs(cmdLine);
                waitFinished();
            }
        }
        else
            isEnd=!cmdProcessor(cmdLine,stdIn);

        QCoreApplication::processEvents();
        if(isEnd)
            break;
    }
    isInScript=tmp;
}

bool Commander::cmdProcessor(QString cmdStr, QTextStream *stdIn)
{
    bool isNext=true;

    cmdStr=cmdStr.trimmed();
    if(cmdStr.isEmpty() || cmdStr[0] == QChar('#'))
        return true;

    QTextStream cmdIn(&cmdStr);
    QString op;
    cmdIn >> op;
    op=op.toLower();
    switch(op[0].toLatin1())
    {
    case 'h':
        if(op == _S("help"))
        {
            writeMsg(tr("Sorry,this function has not been finished yet."));
        }
        else
            unknownCmd(op);
        break;

    case 'j':
        if(op == _S("js"))
        {
            USE_LEFT_ARG(code)
            emit sender->runJs(code);
            waitFinished();
        }
        else if(op == _S("jsconsole"))
        {
            isJsConsoleMode=true;
        }
        else if(op == _S("jssend") || op == _S("jss"))
        {
            USE_LEFT_ARG(code)
            emit sender->jsSend(code);
        }
        else if(op == _S("jsbegin"))
        {
            QString curLine,code;
            while(true)
            {
                qDebug() << ">";
                curLine=stdIn->readLine();
                if(curLine.toLower() == _S("jsend"))
                    break;
                code.append(curLine);
            }
            emit sender->runJs(code);
            waitFinished();
        }
        else if(op == _S("jsend"))
            writeMsg(tr("You have not entered 'JsBegin'. "
                        "Use the command 'Help JsBegin' to get more information.")
                     ,MsgType::Warning);
        else
            unknownCmd(op);
        break;

    case 'g':
        if(op == _S("go"))
        {
            USE_LEFT_ARG(url)
            emit sender->stop();
            emit sender->go(QUrl::fromUserInput(url));
            //sender->isLoading=true;
        }
        else
            unknownCmd(op);
        break;

    case 's':
        if(op == _S("sleep"))
        {
            int sleepTime=-1;
            cmdIn >> sleepTime;
            if(sleepTime >0)
                QThread::msleep(sleepTime);
        }
        else if(op == _S("sleepsec"))
        {
            int sleepTime=-1;
            cmdIn >> sleepTime;
            if(sleepTime >0)
                QThread::sleep(sleepTime);
        }
        else if(op == _S("stop"))
        {
            emit sender->stop();
            //sender->isLoading=false;
        }
        else if(op == _S("setjsworld"))
        {
            int num=-1;
            cmdIn >> num;
            if(num >= 0)
                jsWorld=num;
        }
        else if(op == _S("save"))
        {
            USE_LEFT_ARG(fileName)
            emit sender->save(fileName);
        }
        else
            unknownCmd(op);
        break;

    case 'w':
        if(op == _S("wait"))
        {
            int maxTime=-1;
            cmdIn >> maxTime;
            if(maxTime >= 0)
                sender->waitLoading(maxTime);
            else
                sender->waitLoading();
        }
        else
            unknownCmd(op);
        break;

    case 'e':
        if (op == _S("exit"))
        {
            emit sender->exit();
            isNext=false;
        }
        else if(op == _S("end"))
        {
            if(isInScript)
                isNext=false;
            else
                writeMsg(tr("Command 'End' is only used in scripts."),MsgType::Warning);
        }
        else
            unknownCmd(op);
        break;

    case 'q':
        if (op == _S("quit"))
        {
            emit sender->exit();
            isNext=false;
        }
        else
            unknownCmd(op);
        break;

    case 'b':
        if(op == _S("back"))
        {
            emit sender->back();
        }
        else
            unknownCmd(op);
        break;

    case 'f':
        if(op == _S("forward"))
        {
            emit sender->forward();
        }
        else
            unknownCmd(op);
        break;

    case 'r':
        if(op == _S("reload") || op == _S("refresh"))
        {
            emit sender->reload();
        }
        else if(op == _S("run"))
        {
            USE_LEFT_ARG(runFileName)
            QFileInfo info(runFileName);
            if(!info.exists() || info.suffix() != _S("bcs"))
            {
                QDesktopServices::openUrl(QUrl::fromUserInput(runFileName));
            }
            else
            {
                QFile scriptFile(runFileName);
                if(!scriptFile.exists())
                    writeMsg(tr("Script file no found!"),MsgType::Error);
                else if(!scriptFile.open(QIODevice::ReadOnly|QIODevice::Text))
                    writeMsg(tr("Fail to open Script file!"),MsgType::Error);
                else
                {
                    QTextStream fileIn(&scriptFile);
                    splitCmds(&fileIn,true);
                    scriptFile.close();
                }
            }
        }
        else if(op == _S("removejs"))
        {
            USE_LEFT_ARG(jsFileName)
            emit sender->removeJs(jsFileName);
        }
        else
            unknownCmd(op);
        break;

    case 'p':
        if(op == _S("previoustab") || op == _S("prevtab"))
        {
            emit sender->previousTab();
        }
        else if(op == _S("print"))
        {
            if(isInScript)
            {
                USE_LEFT_ARG(printStr)
                qDebug() << qPrintable(printStr) << "\n";
            }
            else
                writeMsg(tr("Command 'Print' is only used in scripts."),MsgType::Warning);
        }
        else
            unknownCmd(op);
        break;

    case 'n':
        if(op == _S("nexttab"))
        {
            emit sender->nextTab();
        }
        else if(op == _S("newtab"))
        {
            emit sender->newTab();
        }
        else
            unknownCmd(op);
        break;

    case 'o':
        if(op == _S("output"))
        {
            USE_LEFT_ARG(newOutput)
            iohelp.changeOutput(newOutput);
        }
        else
            unknownCmd(op);
        break;

    case 'a':
        if(op == _S("addjs"))
        {
            QString jsType;
            QWebEngineScript::InjectionPoint point;
            cmdIn >> jsType;
            if(jsType.isEmpty())
            {
                moreArgs(op);
                break;
            }
            jsType=jsType.toLower();
            if(jsType == _S("begin"))
                point=QWebEngineScript::InjectionPoint::DocumentCreation;
            else if(jsType == _S("ready"))
                point=QWebEngineScript::InjectionPoint::DocumentReady;
            else if(jsType == _S("deferred"))
                point=QWebEngineScript::InjectionPoint::Deferred;
            else
            {
                badArgs(op);
                break;
            }

            USE_LEFT_ARG(jsFileName)
            QFileInfo info(jsFileName);
            if(!info.exists())
            {
                writeMsg(tr("JS file no found!"),MsgType::Error);
                break;
            }
            QFile jsFile(jsFileName);
            emit sender->addJs(jsFile.readAll(),info.fileName(),point);
        }
        else if(op == _S("about"))
        {
            writeMsg(tr("Sorry,this function has not been finished yet."));
        }
        else
            unknownCmd(op);
        break;

    case 'c':
        if(op == _S("changewindow"))
        {
            int num=-1;
            cmdIn >> num;
            if(num >= 0)
                changeWindow(num);
        }
        else if(op == _S("closetab"))
        {
            emit sender->closeTab();
        }
        else if(op == _S("clearscreen") || op == _S("cls"))
        {
            writeMsg(tr("Sorry,this function has not been finished yet."));
        }
        else if(op == _S("clearcookie"))
        {
            emit sender->clearCookie();
        }
        else if(op == _S("clearcache"))
        {
            emit sender->clearCache();
        }
        else if(op == _S("clearhistory"))
        {
            emit sender->clearHistory();
        }
        else
            unknownCmd(op);
        break;

    case 'l':
        if(op == _S("license"))
        {
            writeMsg(tr("Sorry,this function has not been finished yet."));
        }
        else
            unknownCmd(op);
        break;

    case 'z':
        if(op == _S("zoom"))
        {
            qreal num=0;
            cmdIn >> num;
            if(num >= 0.25 && num <= 5.0)
                emit sender->zoom(num);
        }
        else
            unknownCmd(op);
        break;

    case 'u':
        if(op == _S("useragent"))
        {
            USE_LEFT_ARG(ua)
            emit sender->userAgent(ua);
        }
        else
            unknownCmd(op);
        break;

    default:
        unknownCmd(op);
        break;
    }

    return isNext;
}

void Commander::waitFinished(int checkInterval)
{
    isWaitingFinished=true;
    QCoreApplication::processEvents();
    while(isWaitingFinished)
    {
        QThread::msleep(checkInterval);
        QCoreApplication::processEvents();
    }
    isWaitingFinished=false;
}

void Commander::changeWindow(int id)
{
     BrowserWindow *newWnd=browser->windows().at(id);
     if(newWnd == nullptr)
         return;
     if(curWnd != nullptr)
     {
         disconnect(tabWidget,&QTabWidget::currentChanged,this,0);
     }
     curWnd=newWnd;
     tabWidget=curWnd->tabWidget();
     tabChanged(tabWidget->currentIndex());
     connect(tabWidget,&QTabWidget::currentChanged,this,&Commander::tabChanged);
}

void Commander::sGo(QUrl url)
{
    tabWidget->setUrl(url);
}

void Commander::sBack()
{
    if(curView->isWebActionEnabled(QWebEnginePage::Back))
        tabWidget->triggerWebPageAction(QWebEnginePage::Back);
}

void Commander::sForward()
{
    if(curView->isWebActionEnabled(QWebEnginePage::Forward))
        tabWidget->triggerWebPageAction(QWebEnginePage::Forward);
}

void Commander::sReload()
{
    if(curView->isWebActionEnabled(QWebEnginePage::Reload))
        tabWidget->triggerWebPageAction(QWebEnginePage::Reload);
}

void Commander::sStop()
{
    if(curView->isWebActionEnabled(QWebEnginePage::Stop))
        tabWidget->triggerWebPageAction(QWebEnginePage::Stop);
}

void Commander::sRestart()
{
    writeMsg(tr("Sorry,this function has not been finished yet."));
}

void Commander::sSave(QString path)
{
    curView->page()->save(path);
}

void Commander::sZoom(qreal scale)
{
    curView->setZoomFactor(scale);
}

void Commander::sClearCookie()
{
    QWebEngineProfile::defaultProfile()->cookieStore()->deleteAllCookies();
}

void Commander::sClearHistory()
{
    QWebEngineProfile::defaultProfile()->clearAllVisitedLinks();
    curView->history()->clear();
}

void Commander::sClearCache()
{
    QWebEngineProfile::defaultProfile()->clearAllVisitedLinks();
    QWebEngineProfile::defaultProfile()->clearHttpCache();
}

void Commander::sHistory()
{
    writeMsg(tr("Sorry,this function has not been finished yet."));
}

void Commander::sPreviousTab()
{
    tabWidget->previousTab();
}

void Commander::sNextTab()
{
    tabWidget->nextTab();
}

void Commander::sNewTab()
{
    tabWidget->createTab();
}

void Commander::sCloseTab()
{
    tabWidget->closeTab(tabWidget->currentIndex());
}

void Commander::sUserAgent(QString ua)
{
    QWebEngineProfile::defaultProfile()->setHttpUserAgent(ua);
}

void Commander::sRunJs(QString code)
{
    curView->page()->runJavaScript(code,jsWorld,[this](QVariant data)
    {
        if(data.isNull())
            data=_S("Done.");
        qDebug() << qPrintable(tr("[%1 JsOutput]").arg(IoHelp::dateTimeStr()));
        if(data.canConvert(QMetaType::QString))
            qDebug() << qPrintable(data.toString());
        else
            qDebug() << data;
        qDebug() << "\n";
        emit finished();
    });
}

void Commander::sJsSend(QString code)
{
    curView->page()->runJavaScript(code,jsWorld);
}

void Commander::sAddJs(QString code,QString name,QWebEngineScript::InjectionPoint point)
{
    QWebEngineScript *script =new QWebEngineScript;
    script->setName(name);
    script->setSourceCode(code);
    script->setInjectionPoint(point);
    script->setWorldId(jsWorld);
    userScripts.insert(name,script);
    curView->page()->scripts().insert(*script);
}

void Commander::sRemoveJs(QString name)
{
    if(userScripts.contains(name))
    {
        curView->page()->scripts().remove(*userScripts[name]);
        userScripts.remove(name);
    }
}

void Commander::sExit()
{
    this->quit();
    BrowserWindow *wnd;
    foreach(wnd,browser->windows())
    {
        wnd->close();
    }
    QThread::exit(0);
}

void Commander::tabChanged(int index)
{
    if(index == -1)
        changeWindow(0);
    else
    {
        if(curView != nullptr)
            disconnect(curView,0,sender,0);
        curView=curWnd->currentTab();
        connect(curView,&QWebEngineView::loadStarted,sender,&SignalsHelper::sLoadStart);
        connect(curView,&QWebEngineView::loadFinished,sender,&SignalsHelper::sLoadEnd);
        sender->isLoading=false;
    }
}
