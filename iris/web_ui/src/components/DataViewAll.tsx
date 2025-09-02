// src/components/DataViewAll.tsx
import { useState } from 'react';
import { useFetchLoop } from '../hooks/useFetchLoop';
import type { DataType } from '../types';
import { initialData } from '../data/initialData';

import IASection from './sections/HadesSection';
import GCSection from './sections/GCSection';
import VisionSection from './sections/VisionSection';
import TartarusSection from './sections/TartarusSection';
import CaronteSection from './sections/CaronteSection';
import FieldSection from './sections/FieldSection';
import SkillsSection from './sections/SkillsSection';

import { sendPost } from '../hooks/useSendPost';
import CompetitionSection from './sections/CompetitionSection';

type Props = {
  reading: boolean;
  selected: keyof DataType; // 'skills' já está incluído
  flipField: boolean;
  receptDimensions: boolean;
  setFlipField: React.Dispatch<React.SetStateAction<boolean>>;
  setReceptDimensions: React.Dispatch<React.SetStateAction<boolean>>;

  setSelected: React.Dispatch<React.SetStateAction<keyof DataType>>;
  setSelectedRobotId: React.Dispatch<React.SetStateAction<number | null>>;
  selectedRobotId: number | null;
};

export function DataViewAll({
  reading,
  selected,
  flipField,
  setFlipField,
  receptDimensions,
  setReceptDimensions,
  setSelected,
}: Props) {
  const data = useFetchLoop(reading, initialData);
  const [selectedRobotId, setSelectedRobotId] = useState<number | null>(null);

  const toggleBoolean = async (key: string, currentValue: boolean) => {
    const success = await sendPost('http://localhost:5000/command', {
      [key]: !currentValue,
    });
    if (!success) {
      console.error(`Erro ao alternar ${key}`);
    }
  };

  const updateNumber = async (key: string, value: number) => {
    const success = await sendPost('http://localhost:5000/command', {
      [key]: value,
    });
    if (!success) {
      console.error(`Erro ao atualizar ${key} para ${value}`);
    }
  };

  const section = {
    ia: (
      <IASection
        data={data}
        setSelected={setSelected}
        setSelectedRobotId={setSelectedRobotId}
      />
    ),
    gc: <GCSection data={data} />,
    vision: <VisionSection data={data} />,
    tartarus: (
      <TartarusSection
        data={data}
        updateNumber={updateNumber}
        toggleBoolean={toggleBoolean}
        flipField={flipField}
        setFlipField={setFlipField}
        receptDimensions={receptDimensions}
        setReceptDimensions={setReceptDimensions}
      />
    ),
    caronte: <CaronteSection data={data} />,
    field: <FieldSection data={data} />,
    skills:
      selected === 'skills' ? (
        selectedRobotId !== null ? (
          <SkillsSection
            data={data}
            robotId={selectedRobotId}
            setSelected={setSelected}
          />
        ) : (
          <p>Selecione um robô para ver as skills</p>
        )
      ) : null,
    competition: (
      <CompetitionSection data={data} toggleBoolean={toggleBoolean} />
    ),
  };

  return (
    <div className="ml-0 m-2 p-4 bg-[#545454] text-white border-[#6805F2] border-3 rounded-[5px] w-full max-h-full overflow-y-auto">
      {section[selected]}
    </div>
  );
}
