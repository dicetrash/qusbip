QT += quick

CONFIG += c++17
CONFIG += warn_off

QMAKE_CXXFLAGS += -O
QMAKE_CFLAGS += -O

INCLUDEPATH += $$PWD/ksource
INCLUDEPATH += $$PWD/clientsource

SOURCES += \
        clientsource/groupnotifier.cpp \
        ksource/names.c \
        ksource/sysfs_utils.c \
        ksource/usbip_common.c \
        ksource/usbip_device_driver.c \
        ksource/usbip_host_common.c \
        ksource/usbip_host_driver.c \
        ksource/usbip_list.c \
        ksource/usbip_network.c \
        ksource/usbip_port_list.c \
        ksource/usbipc_attach.c \
        ksource/usbipc_detach.c \
        ksource/utils.c \
        clientsource/coordinator.cpp \
        clientsource/main.cpp \
        clientsource/groupnotifier.cpp \
        clientsource/udevmonitor.cpp \
        clientsource/webbridge.cpp \
        ksource/vhci_driver.c

RESOURCES += qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QTQUICK_COMPILER_SKIPPED_RESOURCES += qml.qrc

DISTFILES += \
    LICENSE \
    README.md

HEADERS += \
    config.h \
    ksource/list.h \
    ksource/names.h \
    ksource/sysfs_utils.h \
    ksource/usbip_common.h \
    ksource/usbip_device_driver.h \
    ksource/usbip_host_common.h \
    ksource/usbip_host_driver.h \
    ksource/utils.h \
    ksource/usbip_network.h \
    clientsource/groupnotifier.h \
    clientsource/coordinator.h \
    clientsource/udevmonitor.h \
    clientsource/webbridge.h \
    ksource/vhci_driver.h

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libudev

unix:!macx: LIBS += -lwrap
