import { useState } from 'react';
import { DataView } from './components/DataView';
import { FieldView } from './components/FieldView';
import { MenuBar } from './components/MenuBar';
import { DataViewAll } from './components/DataViewAll';
import { FIELD_DIMENSIONS, type Division } from './data/fieldDimensions';
import { initialData } from './data/initialData';
import { useFetchLoop } from './hooks/useFetchLoop';
import { mapBallToFieldCoords, mapRobotsToFieldCoords } from './utils';
import type { DataType } from './types';

export type SoftwareOption = 'ia' | 'gc' | 'vision' | 'tartarus' | 'caronte' | 'competition';

export default function App() {
  // App.tsx
  const [flipField, setFlipField] = useState<boolean>(false);
  const [receptDimensions, setReceptDimensions] = useState<boolean>(false);

  const [selectedSoftware, setSelectedSoftware] =
    useState<keyof DataType>('tartarus');

  const [reading, setReading] = useState(false);
  const [selectedDivision, setSelectedDivision] =
    useState<Division>('Entry Level');

  const data = useFetchLoop(reading, initialData);

  let dimensions = initialData.vision.field;

  if (receptDimensions) {
    dimensions = data.vision.field;
  } else {
    dimensions = FIELD_DIMENSIONS[selectedDivision];
  }

  const totalFieldLength = dimensions.field_length + 2 * dimensions.goal_depth;
  const centerX = dimensions.field_width / 2;
  const centerY = totalFieldLength / 2;

  const yellowRobots = mapRobotsToFieldCoords(
    data.vision.robots_yellow,
    centerX,
    centerY,
  );
  const blueRobots = mapRobotsToFieldCoords(
    data.vision.robots_blue,
    centerX,
    centerY,
  );
  const ball = mapBallToFieldCoords(data.vision.balls, centerX, centerY);
  const [selectedRobotId, setSelectedRobotId] = useState<number | null>(null);

  return (
    <div className="bg-[#311A52] h-screen w-screen overflow-hidden">
      <div className="flex flex-col h-screen w-screen">
        <div className="h-auto">
          <MenuBar
            onSelectSoftware={setSelectedSoftware}
            onSelectDivision={setSelectedDivision}
          />
        </div>

        <div className="flex w-full h-full overflow-hidden">
          {/* Campo ajustado pela divisão selecionada */}
          <FieldView
            dimensions={dimensions}
            blueRobots={blueRobots}
            yellowRobots={yellowRobots}
            ball={ball}
            flipField={flipField}
          />

          <div className="flex flex-1 h-full overflow-hidden">
            <DataView reading={reading} setReading={setReading} />
            {selectedSoftware && (
              <DataViewAll
                reading={reading}
                selected={selectedSoftware}
                setSelected={setSelectedSoftware}
                flipField={flipField}
                setFlipField={setFlipField}
                receptDimensions={receptDimensions}
                setReceptDimensions={setReceptDimensions}
                selectedRobotId={selectedRobotId}
                setSelectedRobotId={setSelectedRobotId}
              />
            )}
          </div>
        </div>
      </div>
    </div>
  );
}
