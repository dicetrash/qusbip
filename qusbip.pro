QT += quick

CONFIG += c++17
CONFIG += warn_off

INCLUDEPATH += $$PWD/ksource
INCLUDEPATH += $$PWD/clientsource

SOURCES += \
        ksource/usbip_list.c \
        ksource/usbip_network.c \
        ksource/usbip_port_list.c \
        ksource/usbipc_attach.c \
        ksource/usbipc_detach.c \
        ksource/utils.c \
        clientsource/coordinator.cpp \
        clientsource/main.cpp \
        clientsource/udevmonitor.cpp \
        clientsource/webbridge.cpp

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
    ksource/utils.h \
    ksource/usbip_network.h \
    ksource/debug_macros.h \
    clientsource/coordinator.h \
    clientsource/udevmonitor.h \
    clientsource/webbridge.h

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libudev

unix:!macx: LIBS += -lwrap
unix:!macx: LIBS += -lusbip
