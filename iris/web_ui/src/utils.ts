import type { DetectionBall, DetectionRobot, Robot, RobotField } from './types'; // ajuste o caminho conforme

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

