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
            processo: json.processo,
            estrategia: json.estrategia,
          },
          ia: {
            robots_size: json.robots_size,
            robots: json.robots || [],  // importante: sempre ter array
          },
          vision: {
            timestamp: json.timestamp,
            field_length: json.field_length,
          },
          gc: {
            team_blue: json.team_blue,
            designated_position_x: json.designated_position_x,
            designated_position_y: json.designated_position_y,
            current_command: json.current_command,
            game_event: json.game_event,
            blue: {
              name: json.blue.name,
              score: json.blue.score,
              fouls: json.blue.fouls,
              goalkeeper_id: json.blue.goalkeeper_id,
            },
            yellow: {
              name: json.yellow.name,
              score: json.yellow.score,
              fouls: json.yellow.fouls,
              goalkeeper_id: json.yellow.goalkeeper_id,
            },
          },
          tartarus: {
            ssl_vision: json.ssl_vision,
            team_blue_status: json.team_blue_status,
          },
        });

      } catch (error) {
        console.error('Erro ao buscar dados:', error);
      }
    };

    if (reading) {
      fetchData();
      interval = setInterval(fetchData, 500);
    }

    return () => {
      if (interval) clearInterval(interval);
    };
  }, [reading]);

  return data;
}
