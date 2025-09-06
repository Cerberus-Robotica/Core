import { useEffect, useState } from 'react';
import type { DataType } from '../types';


export const useTartarusState = (tartarus: DataType['tartarus']) => {
  const [stmPort, setStmPort] = useState(tartarus.stm_port ?? 0);
  const [mcastGCPort, setMcastGCPort] = useState(tartarus.mcast_port_gc ?? 0);
  const [mcastSslVisionPort, setMcastSslVisionPort] = useState(
    tartarus.mcast_port_vision_sslvision ?? 0,
  );
  const [mcastTrackedPort, setMcastTrackedPort] = useState(
    tartarus.mcast_port_vision_tracked ?? 0,
  );
  const [camsNumber, setCamsNumber] = useState(tartarus.cams_number ?? 0);
  const [mcastGrsimPort, setMcastGrsimPort] = useState(
    tartarus.mcast_port_vision_grsim ?? 0,
  );

  useEffect(() => {
    setStmPort(tartarus.stm_port ?? 0);
    setMcastGCPort(tartarus.mcast_port_gc ?? 0);
    setMcastSslVisionPort(tartarus.mcast_port_vision_sslvision ?? 0);
    setMcastTrackedPort(tartarus.mcast_port_vision_tracked ?? 0);
    setCamsNumber(tartarus.cams_number ?? 0);
    setMcastGrsimPort(tartarus.cams_number ?? 0);
  }, [
    tartarus.stm_port,
    tartarus.mcast_port_gc,
    tartarus.mcast_port_vision_sslvision,
    tartarus.mcast_port_vision_tracked,
    tartarus.cams_number,
    tartarus.mcast_port_vision_grsim,
  ]);

  return {
    stmPort,
    setStmPort,
    mcastGCPort: mcastGCPort,
    setMcastPortGC: setMcastGCPort,
    mcastSslVisionPort: mcastSslVisionPort,
    setSslVisionPort: setMcastSslVisionPort,
    mcastTrackedPort: mcastTrackedPort,
    setTrackedPort: setMcastTrackedPort,
    camsNumber,
    setCamsNumber,
    mcastGrsimPort: mcastGrsimPort,
    setMcastGrsimPort: setMcastGrsimPort,    
  };
};
