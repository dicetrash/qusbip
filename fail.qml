import QtQuick 2.2
import QtQuick.Dialogs 1.1

MessageDialog {
    id: messageDialog
    title: "kernel module vhci_hcd failed to load"
    text:
" \
This application requires linux kernel module 'vhci_hcd' \n\
because this module failed to load the application must quit now \
"
    onAccepted: {
        Qt.quit()
    }
    Component.onCompleted: visible = true
}
