// src/components/DataViewAll.tsx
import { useFetchLoop } from '../hooks/useFetchLoop';
import type { DataType } from '../types';
import { initialData } from '../data/initialData';

type Props = {
  reading: boolean;
  selected: keyof DataType;
};

export function DataViewAll({ reading, selected }: Props) {
  const data = useFetchLoop(reading, initialData);

  const section = {
    ia: (
      <>
        <h2 className="text-lg font-bold mb-1">IA</h2>
        <p>
          Timestamp: <span className="font-mono">{data.vision.timestamp}</span>
        </p>
        <p>
          Estratégia:{' '}
          <span className="font-mono">{data.caronte.estrategia}</span>
        </p>
        <p>
          Processo: <span className="font-mono">{data.caronte.processo}</span>
        </p>
        <p>
          Robots Size: <span className="font-mono">{data.ia.robots_size}</span>
        </p>

        <div className="mt-2 max-h-[250px] overflow-y-auto space-y-2 border-3 border-[#6805F2] rounded-[5px] p-2 bg-[#545454] shadow-inner">

          {data.ia.robots?.length > 0 ? (
            data.ia.robots.map((robot) => (
              <div key={robot.id} className="p-2 bg-[#2E2E2E] rounded text-sm">
                <p>ID: {robot.id}</p>
                <p>Spinner: {robot.spinner ? 'Sim' : 'Não'}</p>
                <p>Kick: {robot.kick ? 'Sim' : 'Não'}</p>
                <p>Vel. Tangencial: {robot.vel_tang.toFixed(2)}</p>
                <p>Vel. Normal: {robot.vel_normal.toFixed(2)}</p>
                <p>Vel. Angular: {robot.vel_ang.toFixed(2)}</p>
                <p>Kick X: {robot.kick_speed_x.toFixed(2)}</p>
                <p>Kick Z: {robot.kick_speed_z.toFixed(2)}</p>
                <p>Wheel Speed: {robot.wheel_speed ? 'Sim' : 'Não'}</p>
                <p>
                  FR: {robot.wheel_fr.toFixed(2)} | FL:{' '}
                  {robot.wheel_fl.toFixed(2)}
                </p>
                <p>
                  BL: {robot.wheel_bl.toFixed(2)} | BR:{' '}
                  {robot.wheel_br.toFixed(2)}
                </p>
              </div>
            ))
          ) : (
            <p>Nenhum robô identificado</p>
          )}
        </div>
      </>
    ),

    gc: (
      <>
        <h2 className="text-lg font-bold mb-1">Game Controller</h2>
        <p>Team Blue: {data.gc.team_blue ? 'Sim' : 'Não'}</p>
        <p>Designated Position X: {data.gc.designated_position_x}</p>
        <p>Designated Position Y: {data.gc.designated_position_y}</p>
        <p>Current Command: {data.gc.current_command}</p>
        <p>Game Event: {data.gc.game_event}</p>

        <h2 className="font-bold mt-4">Time Azul</h2>
        <p>Nome: {data.gc.blue.name}</p>
        <p>Goleiro ID: {data.gc.blue.goalkeeper_id}</p>
        <p>Faltas: {data.gc.blue.fouls}</p>
        <p>Pontos: {data.gc.blue.score}</p>

        <h2 className="font-bold mt-4">Time Amarelo</h2>
        <p>Nome: {data.gc.yellow.name}</p>
        <p>Goleiro ID: {data.gc.yellow.goalkeeper_id}</p>
        <p>Faltas: {data.gc.yellow.fouls}</p>
        <p>Pontos: {data.gc.yellow.score}</p>
      </>
    ),
    vision: (
      <>
        <h2 className="text-lg font-bold mb-1">Vision</h2>
        <p>
          Timestamp: <span className="font-mono">{data.vision.timestamp}</span>
        </p>
        <p>
          Field Length:{' '}
          <span className="font-mono">{data.vision.field_length}</span>
        </p>
      </>
    ),
    tartarus: (
      <>
        <h2 className="text-lg font-bold mb-1">Tartarus</h2>
        <p>
          SSL Vision:{' '}
          <span className="font-mono">
            {data.tartarus.ssl_vision ? 'Sim' : 'Não'}
          </span>
        </p>
        <p>
          Team Blue Status:{' '}
          <span className="font-mono">
            {data.tartarus.team_blue_status ? 'Sim' : 'Não'}
          </span>
        </p>
      </>
    ),
    caronte: (
      <>
        <h2 className="text-lg font-bold mb-1">Caronte</h2>
        <p>
          Processo: <span className="font-mono">{data.caronte.processo}</span>
        </p>
        <p>
          Estratégia:{' '}
          <span className="font-mono">{data.caronte.estrategia}</span>
        </p>
      </>
    ),
  };

  return (
    <div className="p-2 min-w-[40%]">
      <div className="p-4 bg-[#545454] text-white border-[#6805F2] border-3 rounded-[5px] min-h-full w-full">
        {section[selected]}
      </div>
    </div>
  );
}
