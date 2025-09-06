// src/data/initialData.ts
import type { DataType } from '../types';

export const initialData: DataType = {
  caronte: {
    processo: 'Aguardando dados...',
    estrategia: 'Aguardando dados...',
  },
  ia: {
    robots_size: 0,
    robots: [],
  },
  vision: {
    timestamp: 0,
    balls: { position_x: 0, position_y: 0 },
    field: {
      field_length: 0,
      field_width: 0,
      goal_width: 0,
      goal_depth: 0,
      boundary_width: 0,
      center_circle_radius: 0,
      defense_area_width: 0,
      defense_area_height: 0,
      line_thickness: 0,
      goal_center_to_penalty_mark: 0,
      goal_height: 0,
      ball_radius: 0,
      max_robot_radius: 0,
    },
    robots_yellow: [],
    robots_blue: [],
  },
  gc: {
    team_blue: false,
    gc_designated_position_x: 0,
    gc_designated_position_y: 0,
    gc_current_command: 0,
    gc_game_event: 0,
    blue: {
      name: 'Azul',
      score: 0,
      fouls: 0,
      goalkeeper_id: 0,
    },
    yellow: {
      name: 'Amarelo',
      score: 0,
      fouls: 0,
      goalkeeper_id: 0,
    },
  },
  tartarus: {
    ssl_vision: false,
    autoreferee: false,
    competition_mode: false,
    team_blue: false,
    bool_controller: false,
    debug_mode: false,
    half_field: false,
    iris_as_GC: false,
    stm_port: 0,
    mcast_port_gc: 0,// default 10003
    mcast_port_vision_sslvision: 0, // default 10006
    mcast_port_vision_grsim: 0, // default 10020
    mcast_port_vision_tracked: 0,
    goalkeeper_id: 0,
    cams_number: 0,
  },
  skills: {
    id: 0,
  },
  competition: {
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
  },
  irisGC: {
    designated_position_x: 0,
    designated_position_y: 0,
    current_command: 0,
    game_event: 0,
  }
};
