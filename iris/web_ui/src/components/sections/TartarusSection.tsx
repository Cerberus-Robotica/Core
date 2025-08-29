import { useState } from 'react';
import type { DataType } from '../../types';
import { RowWrapper } from './utilities/RowWrapper';
import { ToggleSwitch } from './utilities/ToggleSwitch';
import { NumberInputRow } from './utilities/NumberInputRow';
import { ActionButton } from './utilities/ActionButton';

type Props = {
  data: DataType;
  updateNumber: (key: string, value: number) => void;
  toggleBoolean: (key: string, currentValue: boolean) => void;
  flipField: boolean;
  receptDimensions: boolean;
  setFlipField: React.Dispatch<React.SetStateAction<boolean>>;
  setReceptDimensions: React.Dispatch<React.SetStateAction<boolean>>;
};

export default function TartarusSection({
  data,
  toggleBoolean,
  updateNumber,
  setFlipField,
  flipField,
  setReceptDimensions,
  receptDimensions,
}: Props) {
  const [stmPort, setStmPort] = useState(data.tartarus.stm_port ?? 0);
  const [mcast_port_gc, setMcast_port_gc] = useState(
    data.tartarus.mcast_port_gc ?? 0,
  );
  const [mcast_port_vision_sslvision, setMcast_port_vision_sslvision] =
    useState(data.tartarus.mcast_port_vision_sslvision ?? 0);
  const [mcast_port_vision_grsim, setMcast_port_vision_grsim] = useState(
    data.tartarus.mcast_port_vision_grsim ?? 0,
  );
  const [mcast_port_vision_tracked, setMcast_port_vision_tracked] = useState(
    data.tartarus.mcast_port_vision_tracked ?? 0,
  );
  const [cams_number, setCams_number] = useState(
    data.tartarus.cams_number ?? 0,
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
        <ToggleSwitch
          value={data.tartarus.ssl_vision}
          onToggle={() => toggleBoolean('ssl_vision', data.tartarus.ssl_vision)}
        />
      </RowWrapper>

      <RowWrapper>
        <p>
          Auto Referee:{' '}
          <span className="font-mono">
            {data.tartarus.autoreferee ? 'Sim' : 'Não'}
          </span>
        </p>
        <ToggleSwitch
          value={data.tartarus.autoreferee}
          onToggle={() =>
            toggleBoolean('autoreferee', data.tartarus.autoreferee)
          }
        />
      </RowWrapper>

      <RowWrapper>
        <p>
          Modo Competição:{' '}
          <span className="font-mono">
            {data.tartarus.competition_mode ? 'Sim' : 'Não'}
          </span>
        </p>
        <ToggleSwitch
          value={data.tartarus.competition_mode}
          onToggle={() =>
            toggleBoolean('competition_mode', data.tartarus.competition_mode)
          }
        />
      </RowWrapper>

      <RowWrapper>
        <p>
          Modo Controller:{' '}
          <span className="font-mono">
            {data.tartarus.bool_controller ? 'Sim' : 'Não'}
          </span>
        </p>
        <ToggleSwitch
          value={data.tartarus.bool_controller}
          onToggle={() =>
            toggleBoolean('bool_controller', data.tartarus.bool_controller)
          }
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
        onSubmit={() =>
          updateNumber(
            'mcast_port_vision_sslvision',
            mcast_port_vision_sslvision,
          )
        }
      />
      <NumberInputRow
        label="GrSim Port:"
        value={mcast_port_vision_grsim}
        setValue={setMcast_port_vision_grsim}
        onSubmit={() =>
          updateNumber('mcast_port_vision_grsim', mcast_port_vision_grsim)
        }
      />
      <NumberInputRow
        label="AutoReferee Port:"
        value={mcast_port_vision_tracked}
        setValue={setMcast_port_vision_tracked}
        onSubmit={() =>
          updateNumber('mcast_port_vision_tracked', mcast_port_vision_tracked)
        }
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

      <NumberInputRow
        label="Número de Cameras:"
        value={cams_number}
        setValue={setCams_number}
        onSubmit={() => updateNumber('cams_number', cams_number)}
      />
    </>
  );
}
