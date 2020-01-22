// SPDX-License-Identifier: GPL-2.0-or-later
#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <QObject>
#include "webbridge.h"
#include "udevmonitor.h"

class Coordinator : public QObject
{
    Q_OBJECT
public:
    explicit Coordinator(WebBridge* bridge, QObject *parent = nullptr);
public slots:
    void processWeb(const QMap<QString, QVariant> &input);
    void processMonitor(const UdevMonitor::UpdateEvent &input);

private:
    WebBridge* bridge;
    UdevMonitor monitor;
};

#endif // COORDINATOR_H
