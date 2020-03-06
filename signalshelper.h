#ifndef SIGNALSHELPER_H
#define SIGNALSHELPER_H

#include <QObject>
#include <QString>
#include <QtDebug>

#include <commander.h>
class Commander;

class SignalsHelper : public QObject
{
    Q_OBJECT
public:
    explicit SignalsHelper(Commander *c);

private:
    friend class Commander;
    Commander *commander;
    bool isLoading=false;
    void waitLoading(int checkInterval=50);

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

#endif // SIGNALSHELPER_H
