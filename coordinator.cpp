// SPDX-License-Identifier: GPL-2.0-or-later
#include "coordinator.h"
#include <QTextStream>
#include "udevmonitor.h"
#include <QVariant>
#include <algorithm>
#include <QVariantMap>
#include <QVariantList>
#include <QMapIterator>
#include <QJsonDocument>
#include <QDebug>
#include <type_traits>

extern "C" {
    #include "ksource/usbip_detach.c"
    #include "ksource/usbip_attach.c"
    #include "ksource/usbip_port_list.c"
    #include "ksource/usbip_list.c"
}

Coordinator::Coordinator(WebBridge *bridge, QObject *parent) : QObject(parent), bridge(bridge), monitor()
{
    connect(bridge, &WebBridge::toApp, this, &Coordinator::processWeb);
    connect(&monitor, &UdevMonitor::update, this, &Coordinator::processMonitor);
    // TODO: tell linux kernel lib owners this segv at second run
    // we will just make this a global once and never free it
    if (usbip_names_init((char*)USBIDS_FILE))
        err("failed to open %s", USBIDS_FILE);
}

// I decided to go with void because I attempted callback
//  through channel and it was always null in js world.
void Coordinator::processWeb(const QVariantMap &input)
{
    auto process = input["process"].toString();

    if (0 == process.compare("detach")) {
        bool ok;
        auto port(input["port"].toInt(&ok));
        if (ok) {
            usbip_detach(port);
        }
        return;
    }

    if (0 == process.compare("attach")) {
        auto host(input["host"].toString().toStdString());
        auto busid(input["busid"].toString().toStdString());

        if (host.compare("") > 0 && busid.compare("") > 0) {
            usbip_attach((char*)host.c_str(), (char*)busid.c_str());
        }
        return;
    }

    if (0 == process.compare("port")) {
        QVariantList devices;
        usbip_devices* linked_device = usbip_port_list();
        usbip_devices* current_device = nullptr;
        while(linked_device != nullptr) {
            devices.push_back(QVariantMap({
                {"port", linked_device->port},
                {"devid", linked_device->devid},
                {"devnum", linked_device->devnum},
                {"busnum", linked_device->busnum},
                {"product_name", linked_device->product_name},
                {"busid", linked_device->udev.busid},
                {"path", linked_device->udev.path},
                {"remote", QVariantMap({
                    {"host", linked_device->host},
                    {"port", linked_device->remote_port},
                    {"busid", linked_device->remote_busid}
                })}
            }));
            current_device = linked_device;
            linked_device = linked_device->next;
            usbip_devices_free(current_device);
        }
        bridge->toWeb(QVariantMap({{"devices", devices}}));
        return;
    }

    if (0 == process.compare("list")) {
        QVariantList devices;
        auto host(input["host"].toString().toStdString());
        usbip_external_list* linked_device = usbip_external_devices((char*)host.c_str());
        usbip_external_list* current_device = nullptr;
        while(linked_device != nullptr) {
            QVariantList interfaces;
            for(int i = 0; i < linked_device->num_interfaces; i++) {
                interfaces.push_back(linked_device->interfaces[i]);
            }
            devices.push_back(QVariantMap({
                {"product_name", linked_device->product_name},
                {"busid", linked_device->busid},
                {"path", linked_device->path},
                {"interfaces", interfaces}
            }));
            current_device = linked_device;
            linked_device = linked_device->next;
            usbip_external_list_free(current_device);
        }
        bridge->toWeb(QVariantMap({
            {"devices", devices},
            {"host", input["host"]}
        }));
    }
}

void Coordinator::processMonitor(const UdevMonitor::UpdateEvent &input)
{
    // This is so that the js knows of usb events so it can react
    bridge->toWeb({
        {"process", "monitor"},
        {"action", input.action},
        {"device", input.device}
    });
}
