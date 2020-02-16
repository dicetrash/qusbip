import * as React from 'react';
import { USBIp } from '../../bridge';
import { Switch } from '@material-ui/core';
type deviceDescriptor = {
    busid: string;
    product: string;
    vendor: string;
    product_name: string;
};

export const USBDevice = (props: {host: string, device: deviceDescriptor}) => {
    const [bound, setBound] = React.useState(false);
    const {host, device} = props;

    const bindAndGo = () => {
        USBIp().publish({
            'process': 'bind',
            host,
            busid: device.busid
        });
        USBIp().publish({
            'process': 'attach',
            host,
            busid: device.busid
        });
        setBound(true);
    };

    return (
      <div>
        <div>Bus: {device.busid}</div>
        <div>Product: {device.product_name}: ({device.vendor}:{device.product})</div>
        <Switch checked={bound} onChange={bindAndGo} />
      </div>
    );
}
