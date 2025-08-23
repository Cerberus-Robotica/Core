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
    designated_position_x: 0,
    designated_position_y: 0,
    current_command: 0,
    game_event: 0,
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
    team_blue_status: false,
    ssl_vision: false,
    autoreferee: false,
    competition_mode: false,
    team_blue: false,
    bool_controller: false,
    stm_port: 0,
    controller_port: 0,
    goalkeeper_id: 0,
  },
};
