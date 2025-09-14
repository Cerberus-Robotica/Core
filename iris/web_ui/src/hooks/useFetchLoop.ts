import { useEffect, useState } from 'react';
import type { DataType } from '../types';
import { competitionData } from '../data/competitionData';

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
            ...initialData.caronte,
            processo: json.processo ?? initialData.caronte.processo,
            estrategia: json.estrategia ?? initialData.caronte.estrategia,
          },
          ia: {
            ...initialData.ia,
            robots_size: json.robots_size ?? initialData.ia.robots_size,
            robots: json.robots ?? initialData.ia.robots,
          },
          vision: {
            ...initialData.vision,
            timestamp: json.timestamp ?? initialData.vision.timestamp,
            balls: json.balls ?? initialData.vision.balls,
            field: json.field ?? initialData.vision.field,
            robots_yellow: json.robots_yellow ?? initialData.vision.robots_yellow,
            robots_blue: json.robots_blue ?? initialData.vision.robots_blue,
          },
          gc: {
            ...initialData.gc,
            team_blue: json.team_blue ?? initialData.gc.team_blue,
            gc_designated_position_x: json.gc_designated_position_x ?? initialData.gc.gc_designated_position_x,
            gc_designated_position_y: json.gc_designated_position_y ?? initialData.gc.gc_designated_position_y,
            gc_current_command: json.gc_current_command ?? initialData.gc.gc_current_command,
            gc_game_event: json.gc_game_event ?? initialData.gc.gc_game_event,
            blue: json.blue ?? initialData.gc.blue,
            yellow: json.yellow ?? initialData.gc.yellow,
          },
          tartarus: (() => {
            const base = {
              ...initialData.tartarus,
              ssl_vision: json.ssl_vision ?? initialData.tartarus.ssl_vision,
              autoreferee: json.autoreferee ?? initialData.tartarus.autoreferee,
              competition_mode: json.competition_mode ?? initialData.tartarus.competition_mode,
              bool_controller: json.bool_controller ?? initialData.tartarus.bool_controller,
              debug_mode: json.debug_mode ?? initialData.tartarus.debug_mode,
              half_field: json.half_field ?? initialData.tartarus.half_field,
              iris_as_GC: json.iris_as_GC ?? initialData.tartarus.iris_as_GC,
              goalkeeper_id: json.goalkeeper_id ?? initialData.tartarus.goalkeeper_id,
              cams_number: json.cams_number ?? initialData.tartarus.cams_number,
              stm_port: json.stm_port ?? initialData.tartarus.stm_port,
              mcast_port_gc: json.mcast_port_gc ?? initialData.tartarus.mcast_port_gc,
              mcast_port_vision_grsim: json.mcast_port_vision_grsim ?? initialData.tartarus.mcast_port_vision_grsim,
              mcast_port_vision_sslvision: json.mcast_port_vision_sslvision ?? initialData.tartarus.mcast_port_vision_sslvision,
              mcast_port_vision_tracked: json.mcast_port_vision_tracked ?? initialData.tartarus.mcast_port_vision_tracked,
              team_blue: json.team_blue ?? initialData.tartarus.team_blue,
            };

            // 🔹 Se ativar o modo competição, sobrescreve com competitionData
            if (base.competition_mode) {
              return { ...base, ...competitionData, competition_mode: true };
            }
            return base;
          })(),
          irisGC: {
            ...initialData.irisGC,
            designated_position_x: json.designated_position_x ?? initialData.irisGC.designated_position_x,
            designated_position_y: json.designated_position_y ?? initialData.irisGC.designated_position_y,
            current_command: json.current_command ?? initialData.irisGC.current_command,
            game_event: json.game_event ?? initialData.irisGC.game_event,
          },
          skills: {
            ...initialData.skills,
            id: json.id ?? initialData.skills.id,
          },
          competition: (() => {
            const competitionMode = json.competition_mode ?? initialData.tartarus.competition_mode;

            // Se modo competição ativo e competition ainda não existe → inicializa com preset
            if (competitionMode) {
              return {
                ...competitionData,
                competition_mode: true,
                autoreferee: json.autoreferee ?? competitionData.autoreferee,
                cams_number: json.cams_number ?? competitionData.cams_number,
                stm_port: json.stm_port ?? competitionData.stm_port,
                mcast_port_gc: json.mcast_port_gc ?? competitionData.mcast_port_gc,
                mcast_port_vision_sslvision: json.mcast_port_vision_sslvision ?? competitionData.mcast_port_vision_sslvision,
                mcast_port_vision_tracked: json.mcast_port_vision_tracked ?? competitionData.mcast_port_vision_tracked,
              };
            }

            // Se modo competição não ativo → mantém apenas competition_mode falso
            return {
              ...competitionData,
              competition_mode: false,
            };
          })(),

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
