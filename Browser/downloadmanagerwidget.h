#ifndef DOWNLOADMANAGERWIDGET_H
#define DOWNLOADMANAGERWIDGET_H

#include "ui_downloadmanagerwidget.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class QWebEngineDownloadItem;
QT_END_NAMESPACE

class DownloadWidget;

class DownloadManagerWidget final : public QWidget, public Ui::DownloadManagerWidget
{
    Q_OBJECT
public:
    explicit DownloadManagerWidget(QWidget *parent = nullptr);

    void downloadRequested(QWebEngineDownloadItem *webItem);

private:
    void add(DownloadWidget *downloadWidget);
    void remove(DownloadWidget *downloadWidget);

    int m_numDownloads;
};

#endif
