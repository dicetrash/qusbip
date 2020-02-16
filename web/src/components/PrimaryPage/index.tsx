
import * as React from "react";
import { USBIp } from '../../bridge';
import { USBHost } from '../USBHost';
import { Button } from '@material-ui/core';
import './PrimaryPage.css';

const hosts: string[] = [];

type hostFound = {
    host: string;
    process: 'hostFound';
}

const isHost = (message: unknown): message is hostFound =>
    typeof message == 'object' && (message as hostFound).process == 'hostFound';

export const PrimaryPage = () => {
    const [, setChildren] = React.useState([]);
    const [finding, setFinding ]= React.useState(hosts.length == 0);
    const monitorMessage = (message: unknown) => {
        if (isHost(message)) {
            setFinding(false);
            if(!hosts.includes(message.host)) {
                hosts.push(message.host);
                setChildren(Object.create(hosts));
            }
        }
    };
    React.useEffect(() => {
        USBIp().subscribe.connect(monitorMessage);
        return () => {
            USBIp().subscribe.disconnect(monitorMessage);
        };
    });

    const doFindHost = () => {
        setFinding(true);
        USBIp().publish({'process': 'find'});
        const findTimer = setInterval(() => {
            if (finding) {
                USBIp().publish({'process': 'find'});
            } else {
                clearInterval(findTimer);
            }
        }, 1000);
    };

    if (finding && hosts.length == 0) {
        USBIp().publish({'process': 'find'});
    }

    return <div>
        <div className="paddedButton">
        {!finding &&
            <Button variant="outlined" onClick={doFindHost}>Find Hosts</Button>
        }
        {finding && 
            <Button variant="outlined" disabled={true}>Searching for Hosts</Button>
        }
        </div>
        {hosts.map((host, index) => 
            <USBHost host={host} key={index}></USBHost>
        )}
    </div>;

}