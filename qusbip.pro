QT += quick

CONFIG += c++17
CONFIG += warn_off

SOURCES += \
        coordinator.cpp \
        ksource/usbip_attach.c \
        ksource/usbip_bind.c \
        ksource/usbip_detach.c \
        ksource/usbip_list.c \
        ksource/usbip_network.c \
        ksource/usbip_port_list.c \
        ksource/usbip_unbind.c \
        ksource/utils.c \
        main.cpp \
        udevmonitor.cpp \
        webbridge.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    LICENSE \
    README.md

HEADERS += \
    coordinator.h \
    ksource/usbip_device_list.h \
    ksource/usbip_network.h \
    ksource/utils.h \
    udevmonitor.h \
    webbridge.h

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libudev

unix:!macx: LIBS += -lwrap
unix:!macx: LIBS += -lusbip
