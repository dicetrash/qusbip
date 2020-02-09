// SPDX-License-Identifier: GPL-2.0-or-later
#include "groupnotifier.h"

#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>

#define FIND_MESSAGE "usbip-find"
#define BIND_PORT_ADM "usbip-bind"
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

void GroupNotifier::bind(QString hostAddr, QString bus)
{
  listener.writeDatagram(QJsonDocument(
     QJsonObject({
       {"message", BIND_PORT_ADM },
       {"bus", bus}
     })
  ).toJson(), QHostAddress(hostAddr), hostPort);
}

void GroupNotifier::dataRecieved()
{
  while(listener.hasPendingDatagrams()) {
    QNetworkDatagram dgram(listener.receiveDatagram());
    if (dgram.data() == FIND_MESSAGE) {
      continue;
    } else if (dgram.data().contains(IM_A_HOST)) {
      emit hostFound(dgram);
    }
    else {
      emit list_arrived(dgram);
    }
  }
}
