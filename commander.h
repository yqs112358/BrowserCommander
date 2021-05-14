#ifndef COMMANDER_H
#define COMMANDER_H

#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QWebEngineScript>

#include "Browser/browser.h"
#include "Browser/browserwindow.h"
#include "Browser/tabwidget.h"
#include "Browser/webview.h"

#include "enginehelper.h"
#include "iohelp.h"

#define CONF_PATH "./BrowserCommander.ini"

class EngineHelper;

class Commander :public QThread
{
    Q_OBJECT
public:
    Commander(Browser *b, QObject *parent=nullptr);
    virtual ~Commander();

    Browser *browser;

    enum MsgType{Info,Warning,Error,Fatal};

protected:
    void writeMsg(QString text,MsgType type=MsgType::Info);
    inline void unknownCmd(QString op);
    inline void badArgs(QString op);
    inline void moreArgs(QString op);

    void run() override;
    void splitCmds(QTextStream *stdIn,bool isScript);
    bool cmdProcessor(QString cmdStr,QTextStream *stdIn);

private:
    friend class EngineHelper;
    BrowserWindow *curWnd=nullptr;
    TabWidget *tabWidget=nullptr;
    WebView *curView=nullptr;

    quint32 jsWorld=0;
    int startWait=5;
    QMap<QString,QWebEngineScript*> userScripts;

    bool isJsConsoleMode=false;
    bool isInScript;
    bool isWaitingFinished=false;
    void waitFinished(int checkInterval=50);
    void changeWindow(int id);

protected slots:
    void sGo(QUrl url);
    void sBack(); void sForward(); void sReload(); void sStop();
    void sSave(QString path);
    void sZoom(qreal scale);
    void sClearCookie(); void sClearHistory(); void sClearCache();
    void sHistory();
    void sPreviousTab(); void sNextTab(); void sNewTab(); void sCloseTab();
    void sUserAgent(QString ua);

    void sRunJs(QString code);
    void sJsSend(QString code);
    void sAddJs(QString code,QString name,QWebEngineScript::InjectionPoint point);
    void sRemoveJs(QString name);

    void sRestart();
    void sExit();

    void tabChanged(int index);

signals:
    void finished();
};

#endif // COMMANDER_H
