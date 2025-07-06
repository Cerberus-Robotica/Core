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
        setData({
          caronte: {
            processo: json.processo,
            estrategia: json.estrategia,
          },
          ia: { robots_size: json.robots_size },
          vision: {
            timestamp: json.timestamp,
            field_length: json.field_length,
          },
          gc: {
            team_blue: json.team_blue,
            score_yellow: json.score_yellow,
            score_blue: json.score_blue,
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
      fetchData(); // inicial
      interval = setInterval(fetchData, 500);
    }

    return () => {
      if (interval) clearInterval(interval);
    };
  }, [reading]);

  return data;
}
