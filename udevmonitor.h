// SPDX-License-Identifier: GPL-2.0-or-later
#ifndef UDEVMONITOR_H
#define UDEVMONITOR_H
#include <QSocketNotifier>
#include <QMap>

class UdevMonitor: public QObject
{
    Q_OBJECT
public:
    UdevMonitor();
    ~UdevMonitor();

    struct UpdateEvent {
        const char* action;
        const char* device;
    };

signals:
    void update(UpdateEvent hash);

private slots:
    void process();

private:
    struct udev *context;
    struct udev_monitor *monitor;
    QSocketNotifier *notifier;
};

#endif // UDEVMONITOR_H