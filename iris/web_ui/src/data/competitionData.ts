// src/data/initialData.ts
import type { TartarusData } from '../types';

export const competitionData: TartarusData = {
    ssl_vision: true,
    autoreferee: false,
    competition_mode: true,
    bool_controller: false,
    debug_mode: false,
    half_field: false,
    iris_as_GC: false,

    goalkeeper_id: 0,
    cams_number: 1,
    stm_port: 0,
    mcast_port_gc: 0,// default 10003
    mcast_port_vision_sslvision: 0, // default 10006
    mcast_port_vision_grsim: 0, // default 10020
    mcast_port_vision_tracked: 0,
    
    team_blue: false,
};
