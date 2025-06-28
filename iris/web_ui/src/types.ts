export type Robots = {
    id: number;
}

export type GameInfos = {
    robots_size: number;
}

export type DataType = {
    caronte: {
      processo: string;
      estrategia: string;
    };
    ia: {
      robots_size: number;
    };
    vision: {
      timestamp: number;
      field_length: number;
    };
    gc: {
      team_blue: boolean;
      score_yellow: number;
      score_blue: number;
    };
    tartarus: {
      ssl_vision: boolean;
      team_blue_status: boolean;
    };
  }