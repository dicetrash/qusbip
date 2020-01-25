# qusbip
A QWeb interface for remote usbip

This project is a Qt + QWebEngine connector for the usbip project
This project is opened to any and all helpers, Merge Requests (MRs) of any amount are considered and only a couple accepted MRs likely are to recieve maintainer access.

## Working
The usbip tools that are exposed as items into the web side are

* attach
* detach
* port
* remote list

## Wanted
Anyone willing to help by making MRs to improve the project.

### Future desires
* Automatic linux kernal module check and loading
* A multicast send from the client.
* A server daemon that runs bind / unbind that is discoverable by multicast and attached by client.
* Platform build docker images for redhat or raspbian.

## Build and Install
### Required development libraries to build
depending on how you want to build it
* qmake / make
* libwrap (tcp-wrappers in arch)
* libusbip (usbip in arch)
* libqt5webengine5 (qt5-webengine in arch)

or if you want a clean directory and environment
* docker

### Build
I have created build.sh that uses docker to build various different packages
run it and follow prompt, the package should show up in the base checkout directory
`build.sh`

### Install
The easiest install method is by package
for hard core arch there is PKGBUILD
for debian/ubuntu we have .deb
and for arch prebuilt we have pkg.tar.xz


### Developing
I generally just use QT creator (of which I am of course unaffiliated) to debug the C/C++ side and vscode with `npm run start:dev` which runs a webpack server
to kind of quickly iterate across the UI side.  I picked using webengine in hopes to open the door for more people to help.
My hope is if you are a C++ developer this helps you learn some React web and if you are a web dev it inspires you to learn some C++/Qt.


## Copying and Using
This project uses portions of linux kernel code with heavy modification
I am unaffiliated with basically anyone and also sort of wish to remain anonymous.
If any portion is not in complaince with license rules please just make an MR to fix it.
This project is protected under GNU GPL v2 see the LICESE file in this project for more details.
