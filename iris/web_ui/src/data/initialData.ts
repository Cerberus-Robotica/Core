// src/data/initialData.ts
import type { DataType } from '../types';

export const initialData: DataType = {
  caronte: { processo: 'Aguardando dados...', estrategia: 'Aguardando dados...' },
  ia: {
    robots_size: 0,
    robots: [], // Inicialmente sem robôs
  },
  vision: { timestamp: 0, field_length: 0 },
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
  tartarus: { ssl_vision: false, team_blue_status: false },
};

