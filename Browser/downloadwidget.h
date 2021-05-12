#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include "ui_downloadwidget.h"

#include <QFrame>
#include <QTime>

QT_BEGIN_NAMESPACE
class QWebEngineDownloadItem;
QT_END_NAMESPACE

class DownloadWidget final : public QFrame, public Ui::DownloadWidget
{
    Q_OBJECT
public:
    explicit DownloadWidget(QWebEngineDownloadItem *download, QWidget *parent = nullptr);

signals:
    void removeClicked(DownloadWidget *self);

private slots:
    void updateWidget();

private:
    QString withUnit(qreal bytes);

    QWebEngineDownloadItem *m_download;
    QTime m_timeAdded;
};

#endif
