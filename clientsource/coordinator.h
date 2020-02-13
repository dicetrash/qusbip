// SPDX-License-Identifier: GPL-2.0-or-later
#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <QObject>
#include <QSettings>
#include <QNetworkDatagram>
#include "webbridge.h"
#include "groupnotifier.h"
#include "udevmonitor.h"

class Coordinator : public QObject
{
    Q_OBJECT
public:
    explicit Coordinator(WebBridge* bridge, QObject *parent = nullptr);
public slots:
    void processWeb(const QMap<QString, QVariant> &input);
    void processMonitor(const UdevMonitor::UpdateEvent &input);
    void sendHost(const QNetworkDatagram datagram);
    void sendDgram(const QNetworkDatagram datagram);

private:
    GroupNotifier* getNotifier();
    WebBridge* bridge;
    UdevMonitor monitor {};
    bool nameInit {false};
    QSettings settings {"AdvancedDynamicsDesign", "qusbip"};
    GroupNotifier* notifier {nullptr};
};

#endif // COORDINATOR_H
