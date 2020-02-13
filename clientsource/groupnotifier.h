#ifndef GROUPNOTIFIER_H
#define GROUPNOTIFIER_H

#include <QString>
#include <QUdpSocket>
#include <QObject>
#include <QNetworkDatagram>

class GroupNotifier: public QObject
{
 Q_OBJECT
public:
  GroupNotifier(QString groupIpv4Host, qint16 hostPort);
  void find();
  void bind(QString hostAddr, QString bus);
  void listAdmin(QString hostAddr);
signals:
  void hostFound(QNetworkDatagram datagram);
  void dgramArrived(QNetworkDatagram datagram);
public slots:
  void dataRecieved();
private:
  QUdpSocket listener;
  QHostAddress groupAddress;
  quint16 hostPort;
};

#endif // GROUPNOTIFIER_H
