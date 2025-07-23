import type { DataType } from '../../types';

type Props = {
  data: DataType;
  toggleBoolean: (key: string, currentValue: boolean) => void;
};

export default function TartarusSection({ data, toggleBoolean }: Props) {
  return (
    <>
      <h2 className="text-lg font-bold mb-4">Tartarus</h2>

      <div className="mb-4 flex items-center justify-between">
        <p>
          SSL Vision: <span className="font-mono">{data.tartarus.ssl_vision ? 'Sim' : 'Não'}</span>
        </p>
        <button
          onClick={() => toggleBoolean('ssl_vision', data.tartarus.ssl_vision)}
          className="px-3 py-1 bg-blue-600 hover:bg-blue-700 text-white rounded"
        >
          Alternar
        </button>
      </div>

      <div className="mb-4 flex items-center justify-between">
        <p>
          Modo Competição: <span className="font-mono">{data.tartarus.competition_mode ? 'Sim' : 'Não'}</span>
        </p>
        <button
          onClick={() => toggleBoolean('competition_mode', data.tartarus.competition_mode)}
          className="px-3 py-1 bg-blue-600 hover:bg-blue-700 text-white rounded"
        >
          Alternar
        </button>
      </div>

      <div className="mb-4 flex items-center justify-between">
        <p>
          Modo Controller: <span className="font-mono">{data.tartarus.bool_controller ? 'Sim' : 'Não'}</span>
        </p>
        <button
          onClick={() => toggleBoolean('bool_controller', data.tartarus.bool_controller)}
          className="px-3 py-1 bg-blue-600 hover:bg-blue-700 text-white rounded"
        >
          Alternar
        </button>
      </div>

      <div className="mb-4 flex items-center justify-between">
        <p>
          Time Azul: <span className="font-mono">{data.tartarus.team_blue ? 'Sim' : 'Não'}</span>
        </p>
      </div>
    </>
  );
}
