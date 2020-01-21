
import * as React from "react";
import { Monitor } from './components/Monitor';
import * as PropTypes from 'prop-types';
import AppBar from '@material-ui/core/AppBar';
import Tabs from '@material-ui/core/Tabs';
import Tab from '@material-ui/core/Tab';
import Typography from '@material-ui/core/Typography';
import Box from '@material-ui/core/Box';
import { ReactNode } from "react";
import './App.css';
import { PrimaryPage } from "./components/PrimaryPage";
import { RawCall } from "./components/RawCall";

function TabPanel(props: {[key: string]: unknown}) {
  const { children, value, index, ...other } = props;

  return (
    <Typography
      component="div"
      role="tabpanel"
      hidden={value !== index}
      id={`simple-tabpanel-${index}`}
      aria-labelledby={`simple-tab-${index}`}
      {...other}
    >
      {value === index && <Box p={3}>{children as ReactNode}</Box>}
    </Typography>
  );
}

TabPanel.propTypes = {
  children: PropTypes.node,
  index: PropTypes.any.isRequired,
  value: PropTypes.any.isRequired,
};

function a11yProps(index: number) {
  return {
    id: `simple-tab-${index}`,
    'aria-controls': `simple-tabpanel-${index}`,
  };
}

export const App = () => {
  const [value, setValue] = React.useState(0);

  const handleChange = (_: unknown, newValue: number) => {
    setValue(newValue);
  };

  return (
    <div className="app">
      <AppBar position="static">
        <Tabs value={value} onChange={handleChange}>
          <Tab label="Host View" {...a11yProps(0)} />
          <Tab label="Raw Call" {...a11yProps(1)} />
        </Tabs>
      </AppBar>
      <TabPanel value={value} index={0} className="setTab">
        <PrimaryPage />
      </TabPanel>
      <TabPanel value={value} index={1} className="setTab">
        <RawCall />
      </TabPanel>
      <Monitor />
    </div>
  );
}