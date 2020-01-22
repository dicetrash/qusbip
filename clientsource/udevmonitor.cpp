// SPDX-License-Identifier: GPL-2.0-or-later
#include <QSocketNotifier>
#include <QException>
#include <libudev.h>
#include "udevmonitor.h"

UdevMonitor::UdevMonitor():
    context(udev_new()),
    monitor(udev_monitor_new_from_netlink(context, "udev"))
{
    if (0 != udev_monitor_filter_add_match_subsystem_devtype(monitor, "usb", nullptr)) {
        throw QException();
    }
    if (0 != udev_monitor_enable_receiving(monitor)) {
        throw QException();
    }

    notifier = new QSocketNotifier(udev_monitor_get_fd(monitor), QSocketNotifier::Read);
    connect(notifier, &QSocketNotifier::activated, this, &UdevMonitor::process);
    notifier->setEnabled(true);
}

UdevMonitor::~UdevMonitor()
{
    delete notifier;
    if (monitor) {
        udev_monitor_unref(monitor);
    }
    udev_unref(context);
}

void UdevMonitor::process()
{
    struct udev_device* device;
    while (nullptr != (device = udev_monitor_receive_device(monitor)))
    {
        emit update({
            .action = udev_device_get_action(device),
            .device = udev_device_get_devnode(device)
        });
        udev_device_unref(device);
    }
}
