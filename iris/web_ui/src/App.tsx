import { useState } from 'react';
import { DataView } from './components/DataView';
import { FieldView } from './components/FieldView';
import { MenuBar } from './components/MenuBar';
import { DataViewAll } from './components/DataViewAll';
import { FIELD_DIMENSIONS, type Division } from './data/fieldDimensions';
import { initialData } from './data/initialData';
import { useFetchLoop } from './hooks/useFetchLoop';
import type { DetectionRobot, RobotField } from './types';

export type SoftwareOption = 'ia' | 'gc' | 'vision' | 'tartarus' | 'caronte';

export default function App() {
  const [selectedSoftware, setSelectedSoftware] =
    useState<SoftwareOption | null>(null);
  const [reading, setReading] = useState(false);
  const [selectedDivision, setSelectedDivision] =
    useState<Division>('Entry Level');

  const data = useFetchLoop(reading, initialData);
  const dimensions = FIELD_DIMENSIONS[selectedDivision];

  const totalFieldLength = dimensions.field_length + 2 * dimensions.goal_depth;
  const centerX = dimensions.field_width / 2;
  const centerY = totalFieldLength / 2;

  // Ajusta coordenadas dos robôs do sistema de visão para coordenadas SVG
  const yellowRobots: RobotField[] = (data.vision.robots_yellow || []).map(
    (dr: DetectionRobot) => ({
      id: dr.robot_id,
      x: centerX + dr.position_y, // soma em vez de subtrair
      y: dr.position_x + centerY,
      orientation: ((dr.orientation * 180) / Math.PI - 90 + 360) % 360,
    }),
  );

  const blueRobots: RobotField[] = (data.vision.robots_blue || []).map(
    (dr: DetectionRobot) => ({
      id: dr.robot_id,
      x: centerX + dr.position_y,
      y: dr.position_x + centerY,
      orientation: ((dr.orientation * 180) / Math.PI - 90 + 360) % 360,
    }),
  );

  return (
    <div className="bg-[#311A52] h-screen w-screen overflow-hidden">
      <div className="flex flex-col h-screen w-screen">
        <div className="h-[35px]">
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
          />

          <div className="flex flex-1 h-full overflow-hidden">
            <DataView reading={reading} setReading={setReading} />
            {selectedSoftware && (
              <DataViewAll reading={reading} selected={selectedSoftware} />
            )}
          </div>
        </div>
      </div>
    </div>
  );
}
