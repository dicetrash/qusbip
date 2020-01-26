#ifndef GROUPNOTIFIER_H
#define GROUPNOTIFIER_H

#include <QString>
#include <QUdpSocket>
#include <QObject>

class GroupNotifier: public QObject
{
 Q_OBJECT
public:
  GroupNotifier(QString groupIpv4Host, qint16 clientPort, qint16 hostPort);
  void transmit();
signals:
  void hostFound(QString hostname);
private:
  QUdpSocket udpSocket4;
  QUdpSocket udpSocket6;
};

#endif // GROUPNOTIFIER_H
