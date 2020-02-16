import * as React from "react";
import {USBIp} from '../../bridge';
import './RawCall.css';

const getInput = (id: string) => {
    return (document.getElementById(id) as any).value as string;
}
const doDelete = () => {
    const port = getInput("port");
    USBIp().publish({'process': 'detach', port});
};
const doAttach = () => {
    const host = getInput("host");
    const busid = getInput("busid");
    USBIp().publish({
        'process': 'attach',
        host,
        busid
    });
};
const doList = () => {
    const host = getInput("host");
    const result = USBIp().publish({
        'process': 'list',
        host
    });
};
const doListAdmin = () => {
    const host = getInput("host");
    USBIp().publish({
        'process': 'listout',
        host
    });
};
const doBindAdmin = () => {
    const host = getInput("host");
    const busid = getInput("busid");
    USBIp().publish({
        'process': 'bind',
        host,
        busid
    });
};
const doPort = () => {
    USBIp().publish({"process": "port"});
};
const doFindHost = () => {
    USBIp().publish({'process': 'find'});
};
export const RawCall = () => (
<div>
    <p>Open up the monitor to see anything from this</p>
    <div className="section">
        <div className="fields">port: <input type="text" id="port" /></div>
        <button onClick={doDelete} className="bluebutton">Delete By Port</button>
    </div>
    <div className="section">
        <div className="fields">host: <input type="text" id="host" /></div>
        <div className="fields">busId: <input type="text" id="busid" /></div>
        <button onClick={doAttach} className="bluebutton">Attach by Host &amp; BusId</button>
        <button onClick={doList} className="bluebutton">List Remote by Host</button>
        <button onClick={doListAdmin} className="bluebutton">As admin list Remote by Host</button>
        <button onClick={doBindAdmin} className="bluebutton">Do admin bind by host and bus</button>
    </div>
    <div className="section">
        <button onClick={doPort} className="bluebutton">List Local Imports</button>
        <button onClick={doFindHost} className="bluebutton">Attempt to find servers</button>
    </div>
</div>);
