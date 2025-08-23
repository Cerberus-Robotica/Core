import { useEffect, useState } from 'react';
import type { DataType } from '../types';

export function useFetchLoop(reading: boolean, initialData: DataType): DataType {
  const [data, setData] = useState<DataType>(initialData);

  useEffect(() => {
    let interval: ReturnType<typeof setInterval>;

    const fetchData = async () => {
      try {
        const res = await fetch('http://localhost:5000/data');
        if (res.status === 204) return;
        const json = await res.json();
        console.log('Dados recebidos do backend:', json);

        setData({
          caronte: {
            processo: json.processo ?? initialData.caronte.processo,
            estrategia: json.estrategia ?? initialData.caronte.estrategia,
          },
          ia: {
            robots_size: json.robots_size ?? 0,
            robots: json.robots ?? [],
          },
          vision: {
            timestamp: json.timestamp ?? 0,
            balls: json.balls ?? { position_x: 0, position_y: 0 },
            field: json.field ?? {
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
            robots_yellow: json.robots_yellow ?? [],
            robots_blue: json.robots_blue ?? [],
          },
          gc: {
            team_blue: json.team_blue ?? false,
            designated_position_x: json.designated_position_x ?? 0,
            designated_position_y: json.designated_position_y ?? 0,
            current_command: json.current_command ?? 0,
            game_event: json.game_event ?? 0,
            blue: {
              name: json.blue?.name ?? '',
              score: json.blue?.score ?? 0,
              fouls: json.blue?.fouls ?? 0,
              goalkeeper_id: json.blue?.goalkeeper_id ?? 0,
            },
            yellow: {
              name: json.yellow?.name ?? '',
              score: json.yellow?.score ?? 0,
              fouls: json.yellow?.fouls ?? 0,
              goalkeeper_id: json.yellow?.goalkeeper_id ?? 0,
            },
          },
          tartarus: {
            ssl_vision: json.ssl_vision ?? false,
            autoreferee: json.autoreferee ?? false,
            team_blue_status: json.team_blue_status ?? false,
            competition_mode: json.competition_mode,
            team_blue: json.team_blue,
            bool_controller: json.bool_controller,
            stm_port: json.stm_port,
            controller_port: json.controller_port,
            goalkeeper_id: json.goalkeeper_id,
          },
        });

      } catch (error) {
        console.error('Erro ao buscar dados:', error);
      }
    };

    if (reading) {
      fetchData();
      interval = setInterval(fetchData, 33);
    } else {
      // Se parar a leitura, pode resetar para initialData para evitar dados antigos
      
    }

    return () => {
      if (interval) clearInterval(interval);
    };
  }, [reading]);

  return data;
}
