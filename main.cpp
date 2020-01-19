// SPDX-License-Identifier: GPL-2.0-or-later
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebEngine/QtWebEngine>
#include <webbridge.h>
#include <QtWebChannel/QWebChannel>
#include <coordinator.h>
#include <QtGlobal>

#define usbip_use_debug true

int main(int argc, char *argv[])
{
    qputenv("QTWEBENGINE_DISABLE_SANDBOX", "1"); // this app will probably run as root
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    qmlRegisterType<WebBridge>("WebBridge", 1, 0, "Bridge");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    Coordinator coord(WebBridge::lastInstance());

    return app.exec();
}
