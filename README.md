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
* Platform build docker images for debian/redhat/arch/raspbian that includes policy kit.

## Build and Install
### Required development libraries to build
* qmake / make
* libwrap (tcp-wrappers in arch)
* libusbip (usbip in arch)
* libqt5webengine5 (qt5-webengine in arch)

### Build
first is the web section
run `cd web && npm ci && npm run build`
then go back down to `cd ../` to the root checkout again and 
run `qmake && make` in the project

### Install
This project requires root to run (or a user capable of usbip commands)
generally after build just
`sudo qusbip` but of course kdesu or gksu work as alternatives
The intent is to eventually make the .deb (debian/ubuntu) and PACKAGE (arch) files with policy kit and desktop service.
If you are willing to help please make an MR, docker images that do this platform agnostic (or cross platform for raspberry pi) would be most appreciated.

### Developing
I generally just use QT creator (of which I am of course unaffiliated) to debug the C/C++ side and vscode with `npm run start:dev` which runs a webpack server
to kind of quickly iterate across the UI side.  I picked using webengine in hopes to open the door for more people to help.
My hope is if you are a C++ developer this helps you learn some React web and if you are a web dev it inspires you to learn some C++/Qt.

interesting commands I run
`docker build -f builds/Dockerfile.arch -t usbqt .`


## Copying and Using
This project uses portions of linux kernel code with heavy modification
I am unaffiliated with basically anyone and also sort of wish to remain anonymous.
If any portion is not in complaince with license rules please just make an MR to fix it.
This project is protected under GNU GPL v2 see the LICESE file in this project for more details.
