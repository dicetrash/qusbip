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
const doPort = () => {
    USBIp().publish({"process": "port"});
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
    </div>
    <div className="section">
        <button onClick={doPort} className="bluebutton">List Local Imports</button>
    </div>
</div>);
