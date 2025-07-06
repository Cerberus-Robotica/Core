import { useState } from 'react';
import type { DataType } from '../types';
import { useFetchLoop } from '../hooks/useFetchLoop';

export function DataView() {
  const [reading, setReading] = useState(false);

  const initialData: DataType = {
    caronte: { processo: 'Aguardando dados...', estrategia: 'Aguardando dados...' },
    ia: { robots_size: 0 },
    vision: { timestamp: 0, field_length: 0 },
    gc: { team_blue: false, score_yellow: 0, score_blue: 0 },
    tartarus: { ssl_vision: false, team_blue_status: false },
  };

  const data = useFetchLoop(reading, initialData);

  return (
    <div className="p-2 w-full md:w-[300px] lg:w-[350px]">
      <div className="p-4 bg-[#545454] text-white border-[#6805F2] border-3 rounded-[5px] w-full">
        <button
          onClick={() => setReading(!reading)}
          className={`mb-4 w-full py-2 rounded-[5px] font-semibold transition-colors duration-200 ${
            reading ? 'bg-red-600 hover:bg-red-700' : 'bg-green-600 hover:bg-green-700'
          } text-white`}
        >
          {reading ? 'Parar leitura' : 'Iniciar leitura'}
        </button>

        <h2 className="text-lg font-bold mb-1">Caronte</h2>
        <p>Processo: <span className="font-mono">{data.caronte.processo}</span></p>
        <p>Estratégia: <span className="font-mono">{data.caronte.estrategia}</span></p>

        <h2 className="text-lg font-bold mt-4 mb-1">IA</h2>
        <p>Robots Size: <span className="font-mono">{data.ia.robots_size}</span></p>

        <h2 className="text-lg font-bold mt-4 mb-1">Vision</h2>
        <p>Timestamp: <span className="font-mono">{data.vision.timestamp}</span></p>
        <p>Field Length: <span className="font-mono">{data.vision.field_length}</span></p>

        <h2 className="text-lg font-bold mt-4 mb-1">Game Controller</h2>
        <p>Team Blue: <span className="font-mono">{data.gc.team_blue ? 'Sim' : 'Não'}</span></p>
        <p>Score Yellow: <span className="font-mono">{data.gc.score_yellow}</span></p>
        <p>Score Blue: <span className="font-mono">{data.gc.score_blue}</span></p>

        <h2 className="text-lg font-bold mt-4 mb-1">Tartarus</h2>
        <p>SSL Vision: <span className="font-mono">{data.tartarus.ssl_vision ? 'Sim' : 'Não'}</span></p>
        <p>Team Blue Status: <span className="font-mono">{data.tartarus.team_blue_status ? 'Sim' : 'Não'}</span></p>
      </div>
    </div>
  );
}
