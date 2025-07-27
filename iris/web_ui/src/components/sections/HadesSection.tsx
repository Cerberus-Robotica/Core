// src/components/sections/IASection.tsx
import type { DataType } from '../../types';

type Props = {
  data: DataType;
};

export default function IASection({ data }: Props) {
  const teamBlueSelected = data.gc.team_blue;

  // IDs dos robôs por time no Vision
  const blueIds = new Set(
    data.vision.robots_blue?.map((r) => r.robot_id) ?? [],
  );
  const yellowIds = new Set(
    data.vision.robots_yellow?.map((r) => r.robot_id) ?? [],
  );

  // Filtra os robôs da IA que estão no time selecionado (pela visão)
  const filteredRobots = data.ia.robots
    .filter((robot) =>
      teamBlueSelected ? blueIds.has(robot.id) : yellowIds.has(robot.id),
    )
    // Remove robôs duplicados (mesmo ID)
    .filter(
      (robot, index, arr) => arr.findIndex((r) => r.id === robot.id) === index,
    )
    // Ordena crescente por ID
    .sort((a, b) => a.id - b.id)
    // Limita pelo robots_size
    .slice(0, data.ia.robots_size);

  const team = teamBlueSelected ? 'blue_team' : 'yellow_team';
  const teamLabel = teamBlueSelected ? 'Azul' : 'Amarelo';

  return (
    <>
      <h2 className="text-lg font-bold mb-1">Hades</h2>
      <p>
        Timestamp: <span className="font-mono">{data.vision.timestamp}</span>
      </p>
      <p>
        Estratégia: <span className="font-mono">{data.caronte.estrategia}</span>
      </p>
      <p>
        Processo: <span className="font-mono">{data.caronte.processo}</span>
      </p>
      <p>
        Robots Size: <span className="font-mono">{data.ia.robots_size}</span>
      </p>

      <p className="text-center font-semibold text-white">
        Time selecionado: {teamLabel}
      </p>
      <div className="mt-2 max-h-[250px] overflow-y-auto space-y-2 border-3 border-[#6805F2] rounded-[5px] p-2 bg-[#545454] shadow-inner">
        {filteredRobots.length > 0 ? (
          filteredRobots.map((robot) => (
            <div
              key={robot.id}
              className="flex items-start gap-4 bg-[#2E2E2E] rounded p-3 text-sm"
            >
              <img
                src={`/img/${team}/id${robot.id}.png`}
                alt={`Robô ${robot.id}`}
                className="w-14 h-14 object-contain shrink-0"
              />
              <div className="grid grid-cols-2 gap-x-4 gap-y-1">
                <p className="col-span-2 font-bold">ID: {robot.id}</p>
                <p>Spinner: {robot.spinner ? 'Sim' : 'Não'}</p>
                <p>Kick: {robot.kick ? 'Sim' : 'Não'}</p>
                <p>Vel. Tangencial: {robot.vel_tang.toFixed(2)}</p>
                <p>Vel. Normal: {robot.vel_normal.toFixed(2)}</p>
                <p>Vel. Angular: {robot.vel_ang.toFixed(2)}</p>
                <p>Kick X: {robot.kick_speed_x.toFixed(2)}</p>
                <p>Kick Z: {robot.kick_speed_z.toFixed(2)}</p>
                <p>Wheel Speed: {robot.wheel_speed ? 'Sim' : 'Não'}</p>
                <p>FR: {robot.wheel_fr.toFixed(2)}</p>
                <p>FL: {robot.wheel_fl.toFixed(2)}</p>
                <p>BL: {robot.wheel_bl.toFixed(2)}</p>
                <p>BR: {robot.wheel_br.toFixed(2)}</p>
              </div>
            </div>
          ))
        ) : (
          <p className="italic text-gray-400">Nenhum robô identificado</p>
        )}
      </div>
    </>
  );
}
