
import * as React from "react";
import './Monitor.css';
import { USBIp } from '../../bridge';
import { ExpansionPanel, ExpansionPanelSummary, ExpansionPanelDetails } from '@material-ui/core';
import Typography from '@material-ui/core/Typography';
import ExpandMoreIcon from '@material-ui/icons/ExpandMore';

const currentChildren: string[] = [];

export const Monitor = () => {
    const [active, setActive] = React.useState(false);
    const toggleState = () => {
        setActive(!active);
    };
    const [children, setChildren] = React.useState([]);
    const monitorMessage = (message: unknown) => {
        currentChildren.push(JSON.stringify(message, null, 2));
        setChildren(Object.create(currentChildren));
    };
    const clear = () => {
        currentChildren.splice(0, currentChildren.length);
        setChildren(Object.create(currentChildren));
    };
    React.useEffect(() => {
        USBIp().subscribe.connect(monitorMessage);
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
          onClick={toggleState}
        >
          <Typography>Monitor</Typography>
        </ExpansionPanelSummary>
        <ExpansionPanelDetails>
            <div>
                {children.map((child, key) => 
                    <pre key={key} className='entry'>{child}</pre>
                )}
            </div>
            <button onClick={clear} className={active ? 'clearBtn active': 'clearBtn'}>clear</button>
        </ExpansionPanelDetails>
      </ExpansionPanel>
    );
}