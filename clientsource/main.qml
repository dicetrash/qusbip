import QtQuick 2.0
import QtQuick.Window 2.0
import QtWebEngine 1.1
import QtWebChannel 1.0
import WebBridge 1.0

Window {
    width: 1024
    height: 750
    visible: true
    Bridge {
        id: bridge
        objectName: "bridge"
        WebChannel.id: "bridge"
    }
    WebChannel {
        id: channel
        registeredObjects: [bridge]
    }
    WebEngineView {
        id: view
        anchors.fill: parent
        url:"qrc:/web/dist/index.html"
        // url: "http://localhost:8080/"
        webChannel: channel
    }
}
