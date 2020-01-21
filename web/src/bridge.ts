
const glob: { [key: string]: any} = window;
glob.pubHold = [];
glob.subHold = [];
glob.USBIp = {
    publish: (ev: unknown) => {
        glob.pubHold.push(ev);
    },
    subscribe: {
        connect: (ev: unknown) => { glob.subHold.push(ev); },
        disconnect: (ev: unknown) => {
            const index = glob.subHold.indexOf(ev);
            if (index > -1) {
                glob.subHold.splice(index, 1);
            }
        }
    }
};

const QBridge = glob.QWebChannel || class {};
const qConst = glob.qt || {};

new QBridge(qConst.webChannelTransport, (channel: any) => {
    const bridge = channel.objects.bridge;
    glob.Bridge = bridge;
    console.log('replacin');
    glob.USBIp = {
        publish: bridge.fromWeb,
        subscribe: bridge.toWeb
    };

    glob.subHold.forEach((callback: unknown) => {
        glob.USBIp.subscribe.connect(callback);
    });
    glob.pubHold.forEach((event: unknown) => {
        glob.USBIp.publish(event);
    });
    delete glob.pubHold;
    delete glob.subHold;
});

const USBIp: () => {
    publish: (ev: unknown) => void;
    subscribe: {
        connect: (ev: unknown) => void;
        disconnect: (ev: unknown) => void;
    }
} = () => { return glob.USBIp };

export { USBIp };