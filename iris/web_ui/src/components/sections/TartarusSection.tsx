import { useState } from 'react';
import type { DataType } from '../../types';

type Props = {
  data: DataType;
  updateNumber: (key: string, value: number) => void;
  toggleBoolean: (key: string, currentValue: boolean) => void;
  flipField: boolean;
  receptDimensions: boolean;
  setFlipField: React.Dispatch<React.SetStateAction<boolean>>;
  setReceptDimensions: React.Dispatch<React.SetStateAction<boolean>>;
};

// Wrapper padrão de linha
function RowWrapper({ children }: { children: React.ReactNode }) {
  return (
    <div className="mb-4 flex items-center justify-between">{children}</div>
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

type NumberInputRowProps = {
  label: string;
  value: number;
  setValue: React.Dispatch<React.SetStateAction<number>>;
  onSubmit: () => void;
};

function NumberInputRow({
  label,
  value,
  setValue,
  onSubmit,
}: NumberInputRowProps) {
  return (
    <RowWrapper>
      <p>{label}</p>
      <div className="flex">
        <input
          type="number"
          value={value}
          onChange={(e) => setValue(Number(e.target.value))}
          className="border rounded px-2 py-1 mr-2 w-24"
        />
        <ActionButton onClick={onSubmit} label="Alterar" />
      </div>
    </RowWrapper>
  );
}

export default function TartarusSection({
  data,
  toggleBoolean,
  updateNumber,
  setFlipField,
  flipField,
  setReceptDimensions,
  receptDimensions,
}: Props) {
  const [stmPort, setStmPort] = useState(data.tartarus.stm_port);
  const [mcast_port_gc, setMcast_port_gc] = useState(
    data.tartarus.mcast_port_gc,
  );
  const [mcast_port_vision_sslvision, setMcast_port_vision_sslvision] =
    useState(data.tartarus.mcast_port_vision_sslvision);
  const [mcast_port_vision_grsim, setMcast_port_vision_grsim] = useState(
    data.tartarus.mcast_port_vision_grsim,
  );
  const [mcast_port_vision_tracked, setMcast_port_vision_tracked] = useState(
    data.tartarus.mcast_port_vision_tracked,
  );

  return (
    <>
      <h2 className="text-lg font-bold mb-4">Tartarus</h2>

      <RowWrapper>
        <p>
          SSL Vision:{' '}
          <span className="font-mono">
            {data.tartarus.ssl_vision ? 'Sim' : 'Não'}
          </span>
        </p>
        <ActionButton
          onClick={() => toggleBoolean('ssl_vision', data.tartarus.ssl_vision)}
          label="Alternar"
        />
      </RowWrapper>

      <RowWrapper>
        <p>
          Auto Referee:{' '}
          <span className="font-mono">
            {data.tartarus.autoreferee ? 'Sim' : 'Não'}
          </span>
        </p>
        <ActionButton
          onClick={() =>
            toggleBoolean('autoreferee', data.tartarus.autoreferee)
          }
          label="Alternar"
        />
      </RowWrapper>

      <RowWrapper>
        <p>
          Modo Competição:{' '}
          <span className="font-mono">
            {data.tartarus.competition_mode ? 'Sim' : 'Não'}
          </span>
        </p>
        <ActionButton
          onClick={() =>
            toggleBoolean('competition_mode', data.tartarus.competition_mode)
          }
          label="Alternar"
        />
      </RowWrapper>

      <RowWrapper>
        <p>
          Modo Controller:{' '}
          <span className="font-mono">
            {data.tartarus.bool_controller ? 'Sim' : 'Não'}
          </span>
        </p>
        <ActionButton
          onClick={() =>
            toggleBoolean('bool_controller', data.tartarus.bool_controller)
          }
          label="Alternar"
        />
      </RowWrapper>

      <RowWrapper>
        <p>
          Time Azul:{' '}
          <span className="font-mono">
            {data.tartarus.team_blue ? 'Sim' : 'Não'}
          </span>
        </p>
      </RowWrapper>

      <h2 className="text-lg font-bold mb-4">Portas</h2>

      <NumberInputRow
        label="STM Port:"
        value={stmPort}
        setValue={setStmPort}
        onSubmit={() => updateNumber('stm_port', stmPort)}
      />
      <NumberInputRow
        label="GC Port:"
        value={mcast_port_gc}
        setValue={setMcast_port_gc}
        onSubmit={() => updateNumber('mcast_port_gc', mcast_port_gc)}
      />
      <NumberInputRow
        label="SSL Vision Port:"
        value={mcast_port_vision_sslvision}
        setValue={setMcast_port_vision_sslvision}
        onSubmit={() => updateNumber('mcast_port_vision_sslvision', mcast_port_vision_sslvision)}
      />
      <NumberInputRow
        label="GrSim Port:"
        value={mcast_port_vision_grsim}
        setValue={setMcast_port_vision_grsim}
        onSubmit={() => updateNumber('mcast_port_vision_grsim', mcast_port_vision_grsim)}
      />
      <NumberInputRow
        label="AutoReferee Port:"
        value={mcast_port_vision_tracked}
        setValue={setMcast_port_vision_tracked}
        onSubmit={() => updateNumber('mcast_port_vision_tracked', mcast_port_vision_tracked)}
      />

      <h2 className="text-lg font-bold mb-4">Campo</h2>

      <RowWrapper>
        <p>Orientação do Campo:</p>
        <ActionButton
          onClick={() => setFlipField(!flipField)} // precisa passar o estado atual para o pai, invertido
          label={flipField ? 'Normal' : 'Inverter'}
        />
      </RowWrapper>

      <RowWrapper>
        <p>Dimensões do Campo:</p>
        <ActionButton
          onClick={() => setReceptDimensions(!receptDimensions)} // precisa passar o estado atual para o pai, invertido
          label={receptDimensions ? 'Fixas' : 'SSL-Vision'}
        />
      </RowWrapper>
    </>
  );
}
