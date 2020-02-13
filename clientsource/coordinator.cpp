// SPDX-License-Identifier: GPL-2.0-or-later
#include "coordinator.h"
#include "udevmonitor.h"
#include "config.h"
#include <QJsonDocument>

extern "C" {
    #include "usbip_common.h"
    #include "usbipc_detach.c"
    #include "usbipc_attach.c"
    #include "usbip_port_list.c"
    #include "usbip_list.c"
}

Coordinator::Coordinator(WebBridge *bridge, QObject *parent) : QObject(parent), bridge(bridge)
{
    connect(bridge, &WebBridge::toApp, this, &Coordinator::processWeb);
    connect(&monitor, &UdevMonitor::update, this, &Coordinator::processMonitor);
}

// I decided to go with void because I attempted callback
//  through channel and it was always null in js world.
void Coordinator::processWeb(const QVariantMap &input)
{
    auto process = input["process"].toString();

    if (!nameInit) {
        // TODO: tell linux kernel lib owners this segv at second run
        // we will just make this a global once and never free
        // TODO: restart on settings save or fix free
        usbip_names_init(const_cast<char*>(settings.value("usbids", USBIDS_FILE).toString().toStdString().c_str()));
    }

    if (0 == process.compare("detach")) {
        bool ok;
        auto port(input["port"].toInt(&ok));
        if (ok) {
            usbipc_detach(port);
        }
        return;
    }

    if (0 == process.compare("attach")) {
        auto host(input["host"].toString().toStdString());
        auto busid(input["busid"].toString().toStdString());

        if (host.compare("") > 0 && busid.compare("") > 0) {
            usbipc_attach((char*)host.c_str(), (char*)busid.c_str());
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
    if (0 == process.compare("settings")) {
        auto save = input["save"].toBool();
        if (save) {
            QMapIterator vals(input["settings"].toMap());
            while(vals.hasNext()) {
                vals.next();
                settings.setValue(vals.key(), vals.value());
            }
        } else {
            QVariantMap output;
            for(auto key: settings.childKeys()) {
                output.insert(key, settings.value(key));
            }
            bridge->toWeb({
                {"process", "settings"},
                {"settings", output}
            });
       }
    }
    if (0 == process.compare("find")) {
       getNotifier()->find();
    }
    if (0 == process.compare("listout")) {
       auto host(input["host"].toString());
       getNotifier()->listAdmin(host);
    }
    if (0 == process.compare("bind")) {
        auto host(input["host"].toString());
        auto bus(input["bus"].toString());
        getNotifier()->bind(host, bus);
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

void Coordinator::sendHost(const QNetworkDatagram datagram)
{
  bridge->toWeb({
    {"process", "hostFound"},
    {"data", datagram.data()},
    {"host", datagram.senderAddress().toString()}
  });
}

void Coordinator::sendDgram(const QNetworkDatagram datagram) {
  bridge->toWeb({
    {"process", "datagram"},
    {"data", QJsonDocument::fromJson(datagram.data())},
    {"host", datagram.senderAddress().toString()}
  });
}

GroupNotifier *Coordinator::getNotifier()
{
  if (notifier == nullptr) {
      const qint16 hostPort = settings.value("multicast/hostPort", 5191).toInt(); // AIM aint usin it
      QString groupIPV4Addr = settings.value("multicast/ipv4addr", "239.255.22.71").toString();
      notifier = new GroupNotifier(groupIPV4Addr, hostPort);
      connect(notifier, &GroupNotifier::hostFound, this, &Coordinator::sendHost);
      connect(notifier, &GroupNotifier::dgramArrived, this, &Coordinator::sendDgram);
   }

  return notifier;
}
