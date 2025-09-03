import { useEffect, useState } from 'react';
import type { DataType } from '../../types';
import { NumberInputRow } from './utilities/NumberInputRow';
import { RowWrapper } from './utilities/RowWrapper';
import { ToggleSwitch } from './utilities/ToggleSwitch';
import { sendPost } from '../../hooks/useSendPost';

type Props = {
  data: DataType;
  toggleBoolean: (key: string, currentValue: boolean) => void;
  updateNumber: (key: string, value: number) => void;
};

export default function CompetitionSection({
  data,
  toggleBoolean,
  updateNumber,
}: Props) {
  const { tartarus } = data;
  // no CompetitionSection
  const [autoreferee, setAutoreferee] = useState(tartarus.autoreferee ?? false);

  // Atualiza sempre que muda o backend, mas respeita alterações do front
  useEffect(() => {
    setAutoreferee((prev) =>
      prev !== autoreferee ? (tartarus.autoreferee ?? false) : prev,
    );
  }, [tartarus.autoreferee]);

  const [stmPort, setStmPort] = useState(tartarus.stm_port ?? 0);
  const [mcastPortGC, setMcastPortGC] = useState(tartarus.mcast_port_gc ?? 0);
  const [sslVisionPort, setSslVisionPort] = useState(
    tartarus.mcast_port_vision_sslvision ?? 0,
  );
  const [trackedPort, setTrackedPort] = useState(
    tartarus.mcast_port_vision_tracked ?? 0,
  );
  const [camsNumber, setCamsNumber] = useState(tartarus.cams_number ?? 0);

  // Atualiza os estados locais sempre que os dados do backend mudarem
  useEffect(() => {
    setStmPort(tartarus.stm_port ?? 0);
    setMcastPortGC(tartarus.mcast_port_gc ?? 0);
    setSslVisionPort(tartarus.mcast_port_vision_sslvision ?? 0);
    setTrackedPort(tartarus.mcast_port_vision_tracked ?? 0);
    setCamsNumber(tartarus.cams_number ?? 0);
  }, [
    tartarus.stm_port,
    tartarus.mcast_port_gc,
    tartarus.mcast_port_vision_sslvision,
    tartarus.mcast_port_vision_tracked,
    tartarus.cams_number,
  ]);

  return (
    <>
      <div className="flex flex-col items-center mb-4">
        <p className="mb-2">
          Modo Competição:{' '}
          <span className="font-mono">
            {tartarus.competition_mode ? 'Sim' : 'Não'}
          </span>
        </p>
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
            <p>
              Auto Referee:{' '}
              <span className="font-mono">
                {tartarus.autoreferee ? 'Sim' : 'Não'}
              </span>
            </p>
            <ToggleSwitch
              value={autoreferee}
              onToggle={async () => {
                const newValue = !autoreferee;
                setAutoreferee(newValue);
                await sendPost('http://localhost:5000/command', {
                  autoreferee: newValue,
                });
              }}
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
            value={mcastPortGC}
            setValue={setMcastPortGC}
            onSubmit={() => updateNumber('mcast_port_gc', mcastPortGC)}
          />
          <NumberInputRow
            label="SSL Vision Port:"
            value={sslVisionPort}
            setValue={setSslVisionPort}
            onSubmit={() =>
              updateNumber('mcast_port_vision_sslvision', sslVisionPort)
            }
          />
          <NumberInputRow
            label="AutoReferee Port:"
            value={trackedPort}
            setValue={setTrackedPort}
            onSubmit={() =>
              updateNumber('mcast_port_vision_tracked', trackedPort)
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
