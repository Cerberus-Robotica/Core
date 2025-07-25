import { useState } from 'react';
import type { DataType } from '../../types';

type Props = {
  data: DataType;
  toggleBoolean: (key: string, currentValue: boolean) => void;
  updateNumber: (key: string, value: number) => void;
};

// Wrapper padrão de linha
function RowWrapper({ children }: { children: React.ReactNode }) {
  return (
    <div className="mb-4 flex items-center justify-between">
      {children}
    </div>
  );
}

// Botão reutilizável com cor padrão azul
function ActionButton({
  onClick,
  label,
}: {
  onClick: () => void;
  label: string;
}) {
  return (
    <button
      onClick={onClick}
      className="w-[100px] px-3 py-1 bg-blue-600 hover:bg-blue-700 text-white rounded"
    >
      {label}
    </button>
  );
}

export default function TartarusSection({
  data,
  toggleBoolean,
  updateNumber,
}: Props) {
  const [stmPort, setStmPort] = useState(data.tartarus.stm_port);
  const [controllerPort, setControllerPort] = useState(data.tartarus.controller_port);

  return (
    <>
      <h2 className="text-lg font-bold mb-4">Tartarus</h2>

      <RowWrapper>
        <p>
          SSL Vision: <span className="font-mono">{data.tartarus.ssl_vision ? 'Sim' : 'Não'}</span>
        </p>
        <ActionButton
          onClick={() => toggleBoolean('ssl_vision', data.tartarus.ssl_vision)}
          label="Alternar"
        />
      </RowWrapper>

      <RowWrapper>
        <p>
          Modo Competição: <span className="font-mono">{data.tartarus.competition_mode ? 'Sim' : 'Não'}</span>
        </p>
        <ActionButton
          onClick={() => toggleBoolean('competition_mode', data.tartarus.competition_mode)}
          label="Alternar"
        />
      </RowWrapper>

      <RowWrapper>
        <p>
          Modo Controller: <span className="font-mono">{data.tartarus.bool_controller ? 'Sim' : 'Não'}</span>
        </p>
        <ActionButton
          onClick={() => toggleBoolean('bool_controller', data.tartarus.bool_controller)}
          label="Alternar"
        />
      </RowWrapper>

      <RowWrapper>
        <p>
          Time Azul: <span className="font-mono">{data.tartarus.team_blue ? 'Sim' : 'Não'}</span>
        </p>
      </RowWrapper>

      <h2 className="text-lg font-bold mb-4">Portas</h2>

      <RowWrapper>
        <p>STM Port:</p>
        <div className="flex">
          <input
            type="number"
            value={stmPort}
            onChange={(e) => setStmPort(Number(e.target.value))}
            className="border rounded px-2 py-1 mr-2 w-24"
          />
          <ActionButton
            onClick={() => updateNumber('stm_port', stmPort)}
            label="Alterar"
          />
        </div>
      </RowWrapper>

      <RowWrapper>
        <p>Controller Port:</p>
        <div className="flex">
          <input
            type="number"
            value={controllerPort}
            onChange={(e) => setControllerPort(Number(e.target.value))}
            className="border rounded px-2 py-1 mr-2 w-24"
          />
          <ActionButton
            onClick={() => updateNumber('controller_port', controllerPort)}
            label="Alterar"
          />
        </div>
      </RowWrapper>
    </>
  );
}
