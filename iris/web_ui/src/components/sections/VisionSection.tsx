// src/components/sections/VisionSection.tsx
import type { DataType, DetectionRobot } from '../../types';

type Props = {
  data: DataType;
};

export default function VisionSection({ data }: Props) {
  return (
    <>
      <h2 className="text-lg font-bold mb-1">Vision</h2>
      <p>
        Timestamp: <span className="font-mono">{data.vision.timestamp}</span>
      </p>

      <h3 className="text-md font-semibold mt-2">Bola</h3>
      {data.vision.balls ? (
        <>
          <p>
            Position X:{' '}
            <span className="font-mono">{data.vision.balls.position_x}</span>
          </p>
          <p>
            Position Y:{' '}
            <span className="font-mono">{data.vision.balls.position_y}</span>
          </p>
        </>
      ) : (
        <p className="italic text-gray-500">Dados da bola indisponíveis.</p>
      )}

      <h3 className="text-md font-semibold mt-2">Campo</h3>
      {data.vision.field ? (
        Object.entries(data.vision.field).map(([key, value]) => (
          <p key={key}>
            {key.replace(/_/g, ' ').replace(/^\w/, (c) => c.toUpperCase())}:{' '}
            <span className="font-mono">{value}</span>
          </p>
        ))
      ) : (
        <p className="italic text-gray-500">Dados de campo indisponíveis.</p>
      )}

      <h3 className="text-md font-semibold mt-2">Robôs Amarelos</h3>
      <div className="max-h-[250px] overflow-y-auto border border-[#6805F2] rounded p-2 bg-[#2E2E2E]">
        {data.vision.robots_yellow?.length ? (
          [...data.vision.robots_yellow]
            .sort((a, b) => a.robot_id - b.robot_id)
            .map((robot: DetectionRobot, index: number) => (
              <div key={index} className="mb-2">
                <p>
                  ID: <span className="font-mono">{robot.robot_id}</span>
                </p>
                <p>
                  Pos X: <span className="font-mono">{robot.position_x}</span>
                </p>
                <p>
                  Pos Y: <span className="font-mono">{robot.position_y}</span>
                </p>
                <p>
                  Orientation:{' '}
                  <span className="font-mono">{robot.orientation}</span>
                </p>
              </div>
            ))
        ) : (
          <p className="italic text-gray-500">
            Nenhum robô amarelo identificado.
          </p>
        )}
      </div>

      <h3 className="text-md font-semibold mt-2">Robôs Azuis</h3>
      <div className="max-h-[250px] overflow-y-auto border border-[#6805F2] rounded p-2 bg-[#2E2E2E]">
        {data.vision.robots_blue?.length ? (
          [...data.vision.robots_blue]
            .sort((a, b) => a.robot_id - b.robot_id)
            .map((robot: DetectionRobot, index: number) => (
              <div key={index} className="mb-2">
                <p>
                  ID: <span className="font-mono">{robot.robot_id}</span>
                </p>
                <p>
                  Pos X: <span className="font-mono">{robot.position_x}</span>
                </p>
                <p>
                  Pos Y: <span className="font-mono">{robot.position_y}</span>
                </p>
                <p>
                  Orientation:{' '}
                  <span className="font-mono">{robot.orientation}</span>
                </p>
              </div>
            ))
        ) : (
          <p className="italic text-gray-500">Nenhum robô azul identificado.</p>
        )}
      </div>
    </>
  );
}
