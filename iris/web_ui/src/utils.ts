import { competitionData } from './data/competitionData';
import { sendPost } from './hooks/useSendPost';
import type { DataType, DetectionBall, DetectionRobot, Robot, RobotField } from './types'; // ajuste o caminho conforme

export function detectionRobotToRobot(dr: DetectionRobot): RobotField {
  return {
    id: dr.robot_id,
    x: dr.position_x,
    y: dr.position_y,
    orientation: (dr.orientation * 180) / Math.PI, // rad → graus
  };
}

export function mapRobotsToFieldCoords(
  robots: DetectionRobot[] | undefined,
  centerX: number,
  centerY: number
): RobotField[] {
  if (!robots) return [];
  return robots.map((dr) => ({
    id: dr.robot_id,
    x: centerX + dr.position_y,
    y: centerY + dr.position_x,
    orientation: ((dr.orientation * 180) / Math.PI - 90 + 360) % 360,
  }));
}

export function mapBallToFieldCoords(
  ball: DetectionBall | undefined,
  centerX: number,
  centerY: number
): { x: number; y: number } | undefined {
  if (!ball) return undefined;
  return {
    x: centerX + ball.position_y,
    y: centerY + ball.position_x,
  };
}

// tipagem mínima para filtro de IDs
type RobotIdOnly = { id: number };

export function filterRobotsForTeam(
  iaRobots: Robot[],
  robotsBlueVision: RobotIdOnly[] | undefined,
  robotsYellowVision: RobotIdOnly[] | undefined,
  teamBlueSelected: boolean,
  maxRobots: number
): Robot[] {
  const blueIds = new Set(robotsBlueVision?.map((r) => r.id) ?? []);
  const yellowIds = new Set(robotsYellowVision?.map((r) => r.id) ?? []);

  return iaRobots
    .filter((robot) => (teamBlueSelected ? blueIds.has(robot.id) : yellowIds.has(robot.id)))
    .filter((robot, index, arr) => arr.findIndex((r) => r.id === robot.id) === index) // remove duplicados
    .sort((a, b) => a.id - b.id)
    .slice(0, maxRobots);
}

export const toggleLocal = async (
  key: keyof DataType['tartarus'],
  value: boolean,
  setValue: React.Dispatch<React.SetStateAction<boolean>>,
) => {
  try {
    const newValue = !value;
    setValue(newValue);

    const success = await sendPost('http://localhost:5000/command', {
      [key]: newValue,
    });

    if (!success) {
      console.error(`Erro ao alternar ${key}`);
      setValue(value); // rollback se deu erro
    }
  } catch (err) {
    console.error(`Erro ao enviar ${key}:`, err);
    setValue(value); // rollback
  }
};

export const toggleBoolean = async (key: string, currentValue: boolean) => {
    try {
      let payload;

      if (key === 'competition_mode') {
        if (!currentValue) {
          // ativando modo competição → manda o preset inteiro
          payload = competitionData;
        } else {
          // desativando → só desliga o campo
          payload = { competition_mode: false };
        }
      } else {
        // demais toggles → comportamento padrão
        payload = { [key]: !currentValue };
      }

      const success = await sendPost('http://localhost:5000/command', payload);

      if (!success) {
        console.error(`Erro ao alternar ${key}`);
      }
    } catch (err) {
      console.error(`Erro ao enviar ${key}:`, err);
    }
  };

  export const updateNumber = async (key: string, value: number) => {
      const success = await sendPost('http://localhost:5000/command', {
        [key]: value,
      });
      if (!success) {
        console.error(`Erro ao atualizar ${key} para ${value}`);
      }
    };
