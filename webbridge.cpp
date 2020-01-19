// SPDX-License-Identifier: GPL-2.0-or-later
#include "webbridge.h"

WebBridge* WebBridge::instance = nullptr;

WebBridge::WebBridge(QObject *parent): QObject(parent) {
    WebBridge::instance = this;
}

WebBridge *WebBridge::lastInstance()
{
    return WebBridge::instance;
}

void WebBridge::fromWeb(const QVariantMap &variant)
{
    emit toApp(variant);
}

void WebBridge::fromApp(const QVariantMap &variant)
{
    emit toWeb(variant);
}
