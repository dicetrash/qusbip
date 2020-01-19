# qusbip
A QWeb interface for remote usbip

This project is a Qt + QWebEngine connector for the usbip project
This project is opened to any and all helpers, MRs of any amount are considered and only a couple accepted MRs likely are to recieve maintainer access.

## Working
The usbip tools that are exposed as items into the web side are

attach

detach

port

remote list

## Wanted
A web developer wanting to help an open source project such as remote usb connections

### Future desires
Automatic linux kernal module check and loading

* A multicast read
* A server side bind / unbind
* A polkit policy file for root run requirements


## Build and Install
### Required development librarys to build
* qmake / make
* libwrap (tcp-wrappers in arch)
* libusbip (usbip in arch)
* libqt5webengine5 (qt5-webengine in arch)

### Build
run `qmake && make` in the project ..

### Install
This project requires root to run (or a user capable of usbip commands)
generally after build just
`sudo qusbip` but of course kdesu or gksu work as alternatives

## Copying and Using
This project uses portions of linux kernel code with heavy modification
I am unaffiliated with that project in any and also am not a lawyer.
If any portion is not in complaince with license rules please just make an MR to fix it.
This project is protected under GNU GPL v2
