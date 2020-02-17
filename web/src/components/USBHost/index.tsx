import * as React from "react";
import { ExpansionPanel, ExpansionPanelSummary, ExpansionPanelDetails } from '@material-ui/core';
import Typography from '@material-ui/core/Typography';
import ExpandMoreIcon from '@material-ui/icons/ExpandMore';
import { USBIp } from '../../bridge';
import { USBDevice } from "../USBDevice";

type deviceDescriptor = {
    busid: string;
    product: string;
    vendor: string;
    vname: string;
    pname: string;
};

type adminDatagram = {
    data: deviceDescriptor[];
    host: string;
    process: 'datagram';
};

const isAdminDatagram = (obj: {[key: string]: unknown}): obj is adminDatagram => obj.process == 'datagram';
export const USBHost = (props: {host: string}) => {
    const [devices, setDevices] = React.useState([]);
    const monitorMessage = (message: {[key: string]: unknown}) => {
        if (isAdminDatagram(message) && message.data.length > 0) {
            setDevices(message.data);
        }
    };
    React.useEffect(() => {
        USBIp().subscribe.connect(monitorMessage);
        if (devices.length == 0) {
            USBIp().publish({
            'process': 'listout',
            host: props.host
        });}
        return () => {
            USBIp().subscribe.disconnect(monitorMessage);
        };
    });

    return (
        <ExpansionPanel className="monitor">
        <ExpansionPanelSummary
          expandIcon={<ExpandMoreIcon />}
          aria-controls="panel1a-content"
          id="panel1a-header"
        >
          <Typography>{props.host}</Typography>
        </ExpansionPanelSummary>
        <ExpansionPanelDetails>
            {devices.map((device: deviceDescriptor, index) => (
                <USBDevice device={device} host={props.host} key={index}></USBDevice>
           ))}
        </ExpansionPanelDetails>
      </ExpansionPanel>
    )
}