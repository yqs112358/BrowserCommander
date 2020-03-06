QT    += core gui webenginewidgets

CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES +=  \
        Browser/browser.cpp \
        Browser/browserwindow.cpp \
        Browser/downloadmanagerwidget.cpp \
        Browser/downloadwidget.cpp \
        Browser/tabwidget.cpp \
        Browser/webpage.cpp \
        Browser/webpopupwindow.cpp \
        Browser/webview.cpp \
        commander.cpp \
        main.cpp \
        outputhelp.cpp \
        signalshelper.cpp

TRANSLATIONS += \
    BrowserCommander_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


HEADERS += \
    Browser/browser.h \
    Browser/browserwindow.h \
    Browser/downloadmanagerwidget.h \
    Browser/downloadwidget.h \
    Browser/tabwidget.h \
    Browser/webpage.h \
    Browser/webpopupwindow.h \
    Browser/webview.h \
    commander.h \
    outputhelp.h \
    signalshelper.h

DISTFILES += \
    Browser/data/3rdparty/COPYING \
    Browser/data/3rdparty/dialog-error.png \
    Browser/data/3rdparty/edit-clear.png \
    Browser/data/3rdparty/go-bottom.png \
    Browser/data/3rdparty/go-next.png \
    Browser/data/3rdparty/go-previous.png \
    Browser/data/3rdparty/process-stop.png \
    Browser/data/3rdparty/qt_attribution.json \
    Browser/data/3rdparty/text-html.png \
    Browser/data/3rdparty/view-refresh.png \
    Browser/data/AppLogoColor.png \
    Browser/data/ninja.png \
    BrowserCommander.ini

RESOURCES += \
    Browser/data/browsercommander.qrc

FORMS += \
    Browser/certificateerrordialog.ui \
    Browser/downloadmanagerwidget.ui \
    Browser/downloadwidget.ui \
    Browser/passworddialog.ui

RC_ICONS = BrowserCommander.ico
