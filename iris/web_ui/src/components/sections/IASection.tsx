// src/components/sections/IASection.tsx
import type { DataType } from '../../types';

type Props = {
  data: DataType;
};

export default function IASection({ data }: Props) {
  return (
    <>
      <h2 className="text-lg font-bold mb-1">IA</h2>
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

      <div className="mt-2 max-h-[250px] overflow-y-auto space-y-2 border-3 border-[#6805F2] rounded-[5px] p-2 bg-[#545454] shadow-inner">
        {data.ia.robots?.length > 0 ? (
          data.ia.robots.map((robot) => (
            <div key={robot.id} className="p-2 bg-[#2E2E2E] rounded text-sm">
              <p>ID: {robot.id}</p>
              <p>Spinner: {robot.spinner ? 'Sim' : 'Não'}</p>
              <p>Kick: {robot.kick ? 'Sim' : 'Não'}</p>
              <p>Vel. Tangencial: {robot.vel_tang.toFixed(2)}</p>
              <p>Vel. Normal: {robot.vel_normal.toFixed(2)}</p>
              <p>Vel. Angular: {robot.vel_ang.toFixed(2)}</p>
              <p>Kick X: {robot.kick_speed_x.toFixed(2)}</p>
              <p>Kick Z: {robot.kick_speed_z.toFixed(2)}</p>
              <p>Wheel Speed: {robot.wheel_speed ? 'Sim' : 'Não'}</p>
              <p>
                FR: {robot.wheel_fr.toFixed(2)} | FL: {robot.wheel_fl.toFixed(2)}
              </p>
              <p>
                BL: {robot.wheel_bl.toFixed(2)} | BR: {robot.wheel_br.toFixed(2)}
              </p>
            </div>
          ))
        ) : (
          <p>Nenhum robô identificado</p>
        )}
      </div>
    </>
  );
}
