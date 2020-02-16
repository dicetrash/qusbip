// SPDX-License-Identifier: GPL-2.0-or-later
#include "groupnotifier.h"

#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>
#include <QHostInfo>

#define FIND_MESSAGE "usbip-find"
#define BIND_PORT_ADM "usbip-bind-adm"
#define IM_A_HOST "usbip-host"
#define LIST_OUT "usbip-list"

GroupNotifier::GroupNotifier(QString groupIpv4Host, qint16 hostPort) : groupAddress(groupIpv4Host), hostPort(hostPort)
{
  listener.bind(QHostAddress::AnyIPv4, hostPort, QUdpSocket::ShareAddress);
  listener.joinMulticastGroup(groupAddress);
  connect(&listener, &QUdpSocket::readyRead, this, &GroupNotifier::dataRecieved);
}

void GroupNotifier::find()
{
  listener.writeDatagram(FIND_MESSAGE, groupAddress, hostPort);
}

void GroupNotifier::listAdmin(QString hostAddr) {
  auto hostResolved = QHostInfo::fromName(hostAddr);
  if (hostResolved.error() != QHostInfo::NoError) {
    return;
  }
  listener.writeDatagram(QJsonDocument(
     QJsonObject({
       {"message", LIST_OUT }
     })
  ).toJson(), hostResolved.addresses().first(), hostPort);
}

void GroupNotifier::bind(QString hostAddr, QString bus)
{
  auto hostResolved = QHostInfo::fromName(hostAddr);
  if (hostResolved.error() != QHostInfo::NoError) {
    return;
  }
  listener.writeDatagram(QJsonDocument(
     QJsonObject({
       {"message", BIND_PORT_ADM },
       {"bus", bus}
     })
  ).toJson(), hostResolved.addresses().first(), hostPort);
}

void GroupNotifier::dataRecieved()
{
  while(listener.hasPendingDatagrams()) {
    QNetworkDatagram dgram(listener.receiveDatagram());
    auto datac(dgram.data());
    if (datac.contains(FIND_MESSAGE)
        || datac.contains(LIST_OUT)
        || datac.contains(BIND_PORT_ADM)) {
      continue;
    } else if (datac.contains(IM_A_HOST)) {
      emit hostFound(dgram);
    }
    else {
      emit dgramArrived(dgram);
    }
  }
}
