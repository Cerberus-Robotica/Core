import type { DataType } from '../../types';
import { RowWrapper } from './utilities/RowWrapper';
import { ToggleSwitch } from './utilities/ToggleSwitch';
import { NumberInputRow } from './utilities/NumberInputRow';
import { ActionButton } from './utilities/ActionButton';
import { CompetitionOverlay } from './utilities/CompetitionOverlay';
import GoalkeeperIdInput from './utilities/GoalkeeperIdInput';
import { useTartarusState } from '../../hooks/useTartarusState';
import { toggleBoolean, updateNumber } from '../../utils';

type Props = {
  data: DataType;
  flipField: boolean;
  receptDimensions: boolean;
  setFlipField: React.Dispatch<React.SetStateAction<boolean>>;
  setReceptDimensions: React.Dispatch<React.SetStateAction<boolean>>;
};

export default function TartarusSection({
  data,
  setFlipField,
  flipField,
  setReceptDimensions,
  receptDimensions,
}: Props) {
  const { tartarus } = data;

  const {
    stmPort,
    setStmPort,
    mcastGCPort: mcastGCPort,
    setMcastPortGC: setMcastGCPort,
    mcastSslVisionPort: mcastSslVisionPort,
    setSslVisionPort: setMcastSslVisionPort,
    mcastTrackedPort: mcastTrackedPort,
    setTrackedPort: setMcastTrackedPort,
    camsNumber,
    setCamsNumber,
    mcastGrsimPort: mcastGrsimPort,
    setMcastGrsimPort: setMcastGrsimPort,
  } = useTartarusState(tartarus);

  return (
    <div className="relative">
      {/* Overlay só aparece se competition_mode estiver ativo */}
      {tartarus.competition_mode && (
        <CompetitionOverlay imageSrc={`/img/Venom.png`} />
      )}

      <h2 className="text-lg font-bold mb-4">Tartarus</h2>

      <RowWrapper>
        <p>SSL Vision: </p>
        <ToggleSwitch
          value={tartarus.ssl_vision}
          onToggle={() => toggleBoolean('ssl_vision', tartarus.ssl_vision)}
        />
      </RowWrapper>

      <RowWrapper>
        <p>Auto Referee: </p>
        <ToggleSwitch
          value={tartarus.autoreferee}
          onToggle={() => toggleBoolean('autoreferee', tartarus.autoreferee)}
        />
      </RowWrapper>

      <RowWrapper>
        <p>Modo Competição: </p>
        <ToggleSwitch
          value={tartarus.competition_mode}
          onToggle={() =>
            toggleBoolean('competition_mode', tartarus.competition_mode)
          }
        />
      </RowWrapper>

      <RowWrapper>
        <p>Modo Controller: </p>
        <ToggleSwitch
          value={tartarus.bool_controller}
          onToggle={() =>
            toggleBoolean('bool_controller', tartarus.bool_controller)
          }
        />
      </RowWrapper>

      <RowWrapper>
        <p>Modo Debug: </p>
        <ToggleSwitch
          value={tartarus.debug_mode}
          onToggle={() => toggleBoolean('debug_mode', tartarus.debug_mode)}
        />
      </RowWrapper>

      <RowWrapper>
        <p>Meio Campo: </p>
        <ToggleSwitch
          value={tartarus.half_field}
          onToggle={() => toggleBoolean('half_field', tartarus.half_field)}
        />
      </RowWrapper>

      <RowWrapper>
        <p>Iris GC: </p>
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

      <GoalkeeperIdInput />

      <h2 className="text-lg font-bold mb-4">Portas</h2>

      <NumberInputRow
        label="STM Port:"
        value={stmPort}
        setValue={setStmPort}
        onSubmit={() => updateNumber('stm_port', stmPort)}
      />
      <NumberInputRow
        label="GC Port:"
        value={mcastGCPort}
        setValue={setMcastGCPort}
        onSubmit={() => updateNumber('mcast_port_gc', mcastGCPort)}
      />
      <NumberInputRow
        label="SSL Vision Port:"
        value={mcastSslVisionPort}
        setValue={setMcastSslVisionPort}
        onSubmit={() =>
          updateNumber(
            'mcast_port_vision_sslvision',
            mcastSslVisionPort,
          )
        }
      />
      <NumberInputRow
        label="GrSim Port:"
        value={mcastGrsimPort}
        setValue={setMcastGrsimPort}
        onSubmit={() =>
          updateNumber('mcast_port_vision_grsim', mcastGrsimPort)
        }
      />
      <NumberInputRow
        label="AutoReferee Port:"
        value={mcastTrackedPort}
        setValue={setMcastTrackedPort}
        onSubmit={() =>
          updateNumber('mcast_port_vision_tracked', mcastTrackedPort)
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
        value={camsNumber}
        setValue={setCamsNumber}
        onSubmit={() => updateNumber('cams_number', camsNumber)}
      />
    </div>
  );
}
