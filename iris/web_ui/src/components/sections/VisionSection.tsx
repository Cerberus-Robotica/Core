import type { DataType, DetectionRobot, RobotField } from '../../types';

type Props = {
  data: DataType;
};

export default function VisionSection({ data }: Props) {
  // Converte os robôs amarelos de DetectionRobot (rad) para Robot (graus)
  const yellowRobots: RobotField[] = (data.vision.robots_yellow || []).map(
    (dr: DetectionRobot) => ({
      id: dr.robot_id,
      x: dr.position_x,
      y: dr.position_y,
      orientation: (dr.orientation * 180) / Math.PI,
    })
  );

  // Converte os robôs azuis de DetectionRobot (rad) para Robot (graus)
  const blueRobots: RobotField[] = (data.vision.robots_blue || []).map(
    (dr: DetectionRobot) => ({
      id: dr.robot_id,
      x: dr.position_x,
      y: dr.position_y,
      orientation: (dr.orientation * 180) / Math.PI,
    })
  );

  return (
    <>
      <h2 className="text-lg font-bold mb-1">Vision</h2>

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
        {yellowRobots.length > 0 ? (
          yellowRobots
            .sort((a, b) => a.id - b.id)
            .map((robot) => (
              <div key={robot.id} className="mb-2">
                <p>
                  ID: <span className="font-mono">{robot.id}</span>
                </p>
                <p>
                  Pos X: <span className="font-mono">{robot.x}</span>
                </p>
                <p>
                  Pos Y: <span className="font-mono">{robot.y}</span>
                </p>
                <p>
                  Orientation:{' '}
                  <span className="font-mono">{robot.orientation.toFixed(2)}°</span>
                </p>
              </div>
            ))
        ) : (
          <p className="italic text-gray-500">Nenhum robô amarelo identificado.</p>
        )}
      </div>

      <h3 className="text-md font-semibold mt-2">Robôs Azuis</h3>
      <div className="max-h-[250px] overflow-y-auto border border-[#6805F2] rounded p-2 bg-[#2E2E2E]">
        {blueRobots.length > 0 ? (
          blueRobots
            .sort((a, b) => a.id - b.id)
            .map((robot) => (
              <div key={robot.id} className="mb-2">
                <p>
                  ID: <span className="font-mono">{robot.id}</span>
                </p>
                <p>
                  Pos X: <span className="font-mono">{robot.x}</span>
                </p>
                <p>
                  Pos Y: <span className="font-mono">{robot.y}</span>
                </p>
                <p>
                  Orientation:{' '}
                  <span className="font-mono">{robot.orientation.toFixed(2)}°</span>
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
