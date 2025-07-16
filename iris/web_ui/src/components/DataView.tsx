import { useState } from 'react';
import { useFetchLoop } from '../hooks/useFetchLoop';
import { sendPost } from '../hooks/useSendPost';
import TeamSelector from './TeamSelector';
import { initialData } from '../data/initialData';

type Props = {
  reading: boolean;
  setReading: React.Dispatch<React.SetStateAction<boolean>>;
};
export function DataView({ reading, setReading }: Props) {
  const [goalieInput, setGoalieInput] = useState<number>(0);

  const data = useFetchLoop(reading, initialData);

  const handleSendGoalie = async () => {
    const success = await sendPost('http://localhost:5000/command', {
      goalkeeper_id: goalieInput,
    });
    alert(success ? 'ID enviado com sucesso' : 'Erro ao enviar ID');
  };

  return (
    <div className="p-2 w-full md:w-[300px] lg:w-[350px]">
      <div className="p-4 bg-[#545454] text-white border-[#6805F2] border-3 rounded-[5px] w-full">
        <button
          onClick={() => setReading(!reading)}
          className={`mb-4 w-full py-2 rounded-[5px] font-semibold transition-colors duration-200 ${
            reading
              ? 'bg-red-600 hover:bg-red-700'
              : 'bg-green-600 hover:bg-green-700'
          } text-white`}
        >
          {reading ? 'Parar leitura' : 'Iniciar leitura'}
        </button>

        <h2 className="text-lg font-bold mb-1">Caronte</h2>
        <p>
          Processo: <span className="font-mono">{data.caronte.processo}</span>
        </p>
        <p>
          Estratégia:{' '}
          <span className="font-mono">{data.caronte.estrategia}</span>
        </p>

        <h2 className="text-lg font-bold mt-4 mb-1">IA</h2>
        <p>
          Robots Size: <span className="font-mono">{data.ia.robots_size}</span>
        </p>

        <h2 className="text-lg font-bold mt-4 mb-1">Vision</h2>
        <p>
          Timestamp: <span className="font-mono">{data.vision.timestamp}</span>
        </p>
        <p>
          Field Length:{' '}
          <span className="font-mono">{data.vision.field_length}</span>
        </p>

        <h2 className="text-lg font-bold mt-4 mb-1">Game Controller</h2>
        <p>
          Team Blue:{' '}
          <span className="font-mono">{data.gc.team_blue ? 'Sim' : 'Não'}</span>
        </p>

        <h2 className="text-lg font-bold mt-4 mb-1">Tartarus</h2>
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

        <div className="mb-4">
          <label className="block mb-1 font-semibold text-white">
            ID do Goleiro:
          </label>
          <input
            type="number"
            className="w-full p-2 rounded border border-gray-300 mb-2 text-black"
            value={goalieInput}
            onChange={(e) => setGoalieInput(Number(e.target.value))}
          />
          <button
            onClick={handleSendGoalie}
            className="w-full py-2 bg-blue-600 hover:bg-blue-700 text-white rounded font-semibold"
          >
            Enviar Goleiro
          </button>
          <TeamSelector/>
          <p>Time Atual: <span className="font-mono">{data.gc.team_blue ? "Azul" : "Amarelo"}</span></p>

        </div>
      </div>
    </div>
  );
}
