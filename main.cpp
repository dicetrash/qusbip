// SPDX-License-Identifier: GPL-2.0-or-later
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <webbridge.h>
#include <coordinator.h>
#include <QProcess>

#define usbip_use_debug true

int main(int argc, char *argv[])
{
    qputenv("QTWEBENGINE_DISABLE_SANDBOX", "1"); // this app will probably run as root
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    bool modprobeFail(QProcess::execute("modprobe vhci_hcd"));
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    if (modprobeFail) {
        engine.load(QUrl(QStringLiteral("qrc:/fail.qml")));
        return app.exec();
    }

    qmlRegisterType<WebBridge>("WebBridge", 1, 0, "Bridge");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    Coordinator coord(WebBridge::lastInstance());

    return app.exec();
}
