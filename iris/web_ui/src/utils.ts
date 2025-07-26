import type { DetectionRobot, RobotField } from './types'; // ajuste o caminho conforme

export function detectionRobotToRobot(dr: DetectionRobot): RobotField {
  return {
    id: dr.robot_id,
    x: dr.position_x,
    y: dr.position_y,
    orientation: (dr.orientation * 180) / Math.PI, // rad → graus
  };
}
