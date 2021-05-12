#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QWebEnginePage>
#include <QWebEngineRegisterProtocolHandlerRequest>

class WebPage : public QWebEnginePage
{
    Q_OBJECT

public:
    WebPage(QWebEngineProfile *profile, QObject *parent = nullptr);

protected:
    bool certificateError(const QWebEngineCertificateError &error) override;

private slots:
    void handleAuthenticationRequired(const QUrl &requestUrl, QAuthenticator *auth);
    void handleFeaturePermissionRequested(const QUrl &securityOrigin, Feature feature);
    void handleProxyAuthenticationRequired(const QUrl &requestUrl, QAuthenticator *auth, const QString &proxyHost);
    void handleRegisterProtocolHandlerRequested(QWebEngineRegisterProtocolHandlerRequest request);
    void handleSelectClientCertificate(QWebEngineClientCertificateSelection clientCertSelection);
};

#endif
