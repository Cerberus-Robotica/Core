import { useState } from 'react';
import type { DataType } from '../../types';
import { NumberInputRow } from './utilities/NumberInputRow';
import { RowWrapper } from './utilities/RowWrapper';
import { ToggleSwitch } from './utilities/ToggleSwitch';
import { toggleBoolean, toggleLocal, updateNumber } from '../../utils';
import { useTartarusState } from '../../hooks/useTartarusState';

type Props = {
  data: DataType;
};

export default function CompetitionSection({ data }: Props) {
  const { tartarus } = data;
  // no CompetitionSection
  const [autoreferee, setAutoreferee] = useState(tartarus.autoreferee ?? false);
  const [sslVision, setSslVision] = useState(tartarus.ssl_vision ?? false);
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
  } = useTartarusState(tartarus);

  return (
    <>
      <div className="flex flex-col items-center mb-4">
        <p className="mb-2">Modo Competição:</p>
        <ToggleSwitch
          value={tartarus.competition_mode}
          onToggle={() =>
            toggleBoolean('competition_mode', tartarus.competition_mode)
          }
        />
      </div>

      {tartarus.competition_mode && (
        <>
          <RowWrapper>
            <p>Auto Referee: </p>
            <ToggleSwitch
              value={autoreferee}
              onToggle={() =>
                toggleLocal('autoreferee', autoreferee, setAutoreferee)
              }
            />
          </RowWrapper>

          <RowWrapper>
            <p>SSL Vision: </p>
            <ToggleSwitch
              value={sslVision}
              onToggle={() =>
                toggleLocal('ssl_vision', sslVision, setSslVision)
              }
            />
          </RowWrapper>

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
              updateNumber('mcast_port_vision_sslvision', mcastSslVisionPort)
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
          <NumberInputRow
            label="Número de Cameras:"
            value={camsNumber}
            setValue={setCamsNumber}
            onSubmit={() => updateNumber('cams_number', camsNumber)}
          />
        </>
      )}
    </>
  );
}
