// SPDX-License-Identifier: GPL-2.0-or-later
#ifndef WEBBRIDGE_H
#define WEBBRIDGE_H

#include <QObject>
#include <QVariantMap>

class WebBridge : public QObject
{
    Q_OBJECT
public:
    explicit WebBridge(QObject *parent = nullptr);
    static WebBridge* lastInstance();

signals:
    void toWeb(const QVariantMap &variant);
    void toApp(const QVariantMap &variant);

public slots:
    void fromWeb(const QVariantMap &variant);
    void fromApp(const QVariantMap &variant);
private:
    static WebBridge* instance;
};

#endif // WEBBRIDGE_H
