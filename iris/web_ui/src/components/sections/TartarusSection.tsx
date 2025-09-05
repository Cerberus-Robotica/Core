import { useState } from 'react';
import type { DataType } from '../../types';
import { RowWrapper } from './utilities/RowWrapper';
import { ToggleSwitch } from './utilities/ToggleSwitch';
import { NumberInputRow } from './utilities/NumberInputRow';
import { ActionButton } from './utilities/ActionButton';
import { CompetitionOverlay } from './utilities/CompetitionOverlay';
import { sendPost } from '../../hooks/useSendPost';

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
  const { tartarus } = data;

  const [stmPort, setStmPort] = useState(tartarus.stm_port ?? 0);
  const [mcast_port_gc, setMcast_port_gc] = useState(
    tartarus.mcast_port_gc ?? 0,
  );
  const [mcast_port_vision_sslvision, setMcast_port_vision_sslvision] =
    useState(tartarus.mcast_port_vision_sslvision ?? 0);
  const [mcast_port_vision_grsim, setMcast_port_vision_grsim] = useState(
    tartarus.mcast_port_vision_grsim ?? 0,
  );
  const [mcast_port_vision_tracked, setMcast_port_vision_tracked] = useState(
    tartarus.mcast_port_vision_tracked ?? 0,
  );
  const [cams_number, setCams_number] = useState(
    tartarus.cams_number ?? 0,
  );
  const [goalieInput, setGoalieInput] = useState<number>(0);

  return (
    <div className="relative">
      {/* Overlay só aparece se competition_mode estiver ativo */}
      {tartarus.competition_mode && (
        <CompetitionOverlay imageSrc={`/img/Venom.png`} />
      )}

      <h2 className="text-lg font-bold mb-4">Tartarus</h2>

      <RowWrapper>
        <p>
          SSL Vision:{' '}
        </p>
        <ToggleSwitch
          value={tartarus.ssl_vision}
          onToggle={() => toggleBoolean('ssl_vision', tartarus.ssl_vision)}
        />
      </RowWrapper>

      <RowWrapper>
        <p>
          Auto Referee:{' '}
        </p>
        <ToggleSwitch
          value={tartarus.autoreferee}
          onToggle={() =>
            toggleBoolean('autoreferee', tartarus.autoreferee)
          }
        />
      </RowWrapper>

      <RowWrapper>
        <p>
          Modo Competição:{' '}
        </p>
        <ToggleSwitch
          value={tartarus.competition_mode}
          onToggle={() =>
            toggleBoolean('competition_mode', tartarus.competition_mode)
          }
        />
      </RowWrapper>

      <RowWrapper>
        <p>
          Modo Controller:{' '}
        </p>
        <ToggleSwitch
          value={tartarus.bool_controller}
          onToggle={() =>
            toggleBoolean('bool_controller', tartarus.bool_controller)
          }
        />
      </RowWrapper>

      <RowWrapper>
        <p>
          Modo Debug:{' '}
        </p>
        <ToggleSwitch
          value={tartarus.debug_mode}
          onToggle={() => toggleBoolean('debug_mode', tartarus.debug_mode)}
        />
      </RowWrapper>

      <RowWrapper>
        <p>
          Meio Campo:{' '}
        </p>
        <ToggleSwitch
          value={tartarus.half_field}
          onToggle={() => toggleBoolean('half_field', tartarus.half_field)}
        />
      </RowWrapper>

      <RowWrapper>
        <p>
          Iris como GC:{' '}
        </p>
        <ToggleSwitch
          value={tartarus.iris_as_GC}
          onToggle={() => toggleBoolean('iris_as_GC', tartarus.iris_as_GC)}
        />
      </RowWrapper>

      <RowWrapper>
        <p>
          Time Azul:{' '}
          <span className="font-mono">
            {tartarus.team_blue ? 'Sim' : 'Não'}
          </span>
        </p>
      </RowWrapper>


      <NumberInputRow
        label="ID do Goleiro:"
        value={goalieInput}
        setValue={setGoalieInput}
        onSubmit={async () => {
          await sendPost('http://localhost:5000/command', {
            goalkeeper_id: goalieInput,
            goalkeeper_from_lcm: false, // essencial para impedir sobrescrita
          });
        }}
      />

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
    </div>
  );
}
