#ifndef ENGINEHELPER_H
#define ENGINEHELPER_H

#include <QObject>
#include <QString>
#include <QtDebug>
#include <QJSEngine>

#include "commander.h"
class Commander;

class EngineHelper : public QObject
{
    Q_OBJECT
public:
    explicit EngineHelper(Commander *c);

private:
    friend class Commander;
    Commander *commander;

    QJSEngine engine;
    bool isLoading=false;
    void waitLoading(int maxTime=-1,int checkInterval=50);

signals:
    void go(QUrl url);
    void back(); void forward(); void reload(); void stop();
    void save(QString path);
    void zoom(qreal scale);
    void clearCookie(); void clearHistory(); void clearCache();
    void history();
    void previousTab(); void nextTab(); void newTab(); void closeTab();
    void userAgent(QString ua);

    void runJs(QString code);
    void jsSend(QString code);
    void addJs(QString code,QString name,QWebEngineScript::InjectionPoint point);
    void removeJs(QString name);

    void restart();
    void exit();

protected slots:
    void sFinished();
    void sLoadStart();
    void sLoadEnd();
};

#endif // ENGINEHELPER_H
