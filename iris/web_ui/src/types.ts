export type Robots = {
    id: number;
}

export type GameInfos = {
    robots_size: number;
}

export type TeamInfo = {
  name: string;
  score: number;
  fouls: number;
  goalkeeper_id: number;
};

export type GCType = {
  team_blue: boolean;
  designated_position_x: number;
  designated_position_y: number;
  current_command: number;
  game_event: number;
  blue: TeamInfo;
  yellow: TeamInfo;
};

export type Robot = {
  id: number;
  spinner: boolean;
  kick: boolean;
  vel_tang: number;
  vel_normal: number;
  vel_ang: number;
  kick_speed_x: number;
  kick_speed_z: number;
  wheel_speed: boolean;
  wheel_fr: number;
  wheel_fl: number;
  wheel_bl: number;
  wheel_br: number;
};

export type IAData = {
  robots_size: number;
  robots: Robot[];
};

export type DataType = {
  caronte: {
    processo: string;
    estrategia: string;
  };
  ia: IAData;
  vision: {
    timestamp: number;
    field_length: number;
  };
  gc: GCType;
  tartarus: {
    ssl_vision: boolean;
    team_blue_status: boolean;
  };
};
