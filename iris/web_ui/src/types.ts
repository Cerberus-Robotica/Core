// --- IA ---

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

export type CompetitionData = {
  ssl_vision: boolean;
  autoreferee: boolean,
  competition_mode: boolean;
  bool_controller: boolean;
  debug_mode: boolean;
  half_field: boolean;
  iris_as_GC: boolean;

  goalkeeper_id: number;
  cams_number: number;
  stm_port: number;
  mcast_port_gc: number;
  mcast_port_vision_sslvision: number;
  mcast_port_vision_grsim: number;
  mcast_port_vision_tracked: number;
  
  team_blue: boolean,
}

export type SkillsData = {
  id: number;
}

export type IAData = {
  robots_size: number;
  robots: Robot[];
};

// --- Game Controller ---

export type TeamInfo = {
  name: string;
  score: number;
  fouls: number;
  goalkeeper_id: number;
};

export type GCType = {
  team_blue: boolean;
  gc_designated_position_x: number;
  gc_designated_position_y: number;
  gc_current_command: number;
  gc_game_event: number;
  blue: TeamInfo;
  yellow: TeamInfo;
};

// --- Vision ---

export type DetectionRobot = {
  robot_id: number;
  position_x: number; // mm
  position_y: number; // mm
  orientation: number; // rad
};

export type RobotField = {
  id: number;
  x: number;
  y: number;
  orientation: number; // graus
}

export type DetectionBall = {
  position_x: number; // mm
  position_y: number; // mm
};

export type BallField = {
  x: number;
  y: number;
};

export type DetectionGeometry = {
  field_length: number;
  field_width: number;
  goal_width: number;
  goal_depth: number;
  boundary_width: number;
  center_circle_radius: number;
  defense_area_width: number;
  defense_area_height: number;
  line_thickness: number;
  goal_center_to_penalty_mark: number;
  goal_height: number;
  ball_radius: number;
  max_robot_radius: number;
};

export type FieldDimensions = {
  [key: string]: DetectionGeometry;
}

export type VisionData = {
  timestamp: number;
  robots_yellow: DetectionRobot[];
  robots_blue: DetectionRobot[];
  balls: DetectionBall;
  field: DetectionGeometry;
};

// --- Tartarus (AutoRef) ---

export type TartarusData = {
  ssl_vision: boolean;
  autoreferee: boolean;
  competition_mode: boolean;
  team_blue: boolean;
  bool_controller: boolean;
  debug_mode: boolean;
  half_field: boolean;
  iris_as_GC: boolean;
  stm_port: number;
  mcast_port_gc: number;// default 10003
	mcast_port_vision_sslvision: number; // default 10006
	mcast_port_vision_grsim: number; // default 10020
	mcast_port_vision_tracked: number;
  goalkeeper_id: number;
  cams_number: number;
};

export type IrisGCData = {
  designated_position_x: number;
  designated_position_y: number;
  current_command: number;
  game_event: number;
}

// --- Caronte (Processo / Estratégia) ---

export type CaronteData = {
  processo: string;
  estrategia: string;
};

// --- Dado completo ---

export type DataType = {
  caronte: CaronteData;
  ia: IAData;
  vision: VisionData;
  gc: GCType;
  tartarus: TartarusData;
  skills: SkillsData;
  competition: CompetitionData;
  irisGC: IrisGCData;
};

// --- BackendData (estrutura parcial vinda do backend) ---
export type BackendData = Partial<{
  caronte: Partial<CaronteData>;
  ia: Partial<IAData>;
  vision: Partial<VisionData>;
  gc: Partial<GCType>;
  tartarus: Partial<TartarusData>;
}>;

