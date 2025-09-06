import type { FieldProps } from './FieldView';

export type Robot = {
  id: number;
  x: number;
  y: number;
  orientation?: number; // graus, opcional
};

export function FieldSVG({
  dimensions,
  blueRobots = [],
  yellowRobots = [],
  ball,
  flipField = false,
}: FieldProps) {
  const totalFieldLength = dimensions.field_length + 2 * dimensions.goal_depth;
  const centerX = dimensions.field_width / 2;
  const centerY = totalFieldLength / 2;
  const goalTopY = 0;
  const goalBottomY = totalFieldLength - dimensions.goal_depth;
  const strokeWidth = 10;
  const robotSize = dimensions.max_robot_radius * 2;
  const ballSize = dimensions.ball_radius * 2;
  console.log(
    'flipField:',
    flipField,
    'centerX:',
    centerX,
    'centerY:',
    centerY,
  );

  return (
    <svg
      className="h-full w-auto"
      viewBox={`${-dimensions.boundary_width} 0 ${dimensions.field_width + 2 * dimensions.boundary_width} ${totalFieldLength}`}
      preserveAspectRatio="xMidYMid meet"
    >
      <g
        transform={
          flipField ? `rotate(180, ${centerX}, ${centerY})` : undefined
        }
      >
        {/* Fundo campo + gol (centralizado com largura do gol) */}
        <rect
          x={(dimensions.field_width - dimensions.goal_width) / 2}
          y={0}
          width={dimensions.goal_width}
          height={totalFieldLength}
          fill="#545454"
        />

        {/* Campo jogável */}
        <rect
          x={0}
          y={dimensions.goal_depth}
          width={dimensions.field_width}
          height={dimensions.field_length}
          stroke="white"
          strokeWidth={dimensions.line_thickness}
          fill="#545454"
        />

        {/* Linha do meio campo */}
        <line
          x1={0}
          y1={centerY}
          x2={dimensions.field_width}
          y2={centerY}
          stroke="white"
          strokeWidth={dimensions.line_thickness}
        />

        {/* Circulo central */}
        <circle
          cx={centerX}
          cy={centerY}
          r={dimensions.center_circle_radius}
          stroke="white"
          strokeWidth={dimensions.line_thickness}
          fill="transparent"
        />

        {/* Área de defesa superior */}
        <rect
          x={(dimensions.field_width - dimensions.defense_area_width) / 2}
          y={dimensions.goal_depth}
          width={dimensions.defense_area_width}
          height={dimensions.defense_area_height}
          stroke="white"
          strokeWidth={dimensions.line_thickness}
          fill="transparent"
        />

        {/* Área de defesa inferior */}
        <rect
          x={(dimensions.field_width - dimensions.defense_area_width) / 2}
          y={
            dimensions.goal_depth +
            dimensions.field_length -
            dimensions.defense_area_height
          }
          width={dimensions.defense_area_width}
          height={dimensions.defense_area_height}
          stroke="white"
          strokeWidth={dimensions.line_thickness}
          fill="transparent"
        />

        {/* Gol superior */}
        <line
          x1={(dimensions.field_width - dimensions.goal_width) / 2}
          y1={goalTopY}
          x2={(dimensions.field_width + dimensions.goal_width) / 2}
          y2={goalTopY}
          stroke="#8D00F2"
          strokeWidth={strokeWidth}
        />
        <line
          x1={(dimensions.field_width - dimensions.goal_width) / 2}
          y1={goalTopY}
          x2={(dimensions.field_width - dimensions.goal_width) / 2}
          y2={goalTopY + dimensions.goal_depth}
          stroke="#8D00F2"
          strokeWidth={strokeWidth}
        />
        <line
          x1={(dimensions.field_width + dimensions.goal_width) / 2}
          y1={goalTopY}
          x2={(dimensions.field_width + dimensions.goal_width) / 2}
          y2={goalTopY + dimensions.goal_depth}
          stroke="#8D00F2"
          strokeWidth={strokeWidth}
        />

        {/* Gol inferior */}
        <line
          x1={(dimensions.field_width - dimensions.goal_width) / 2}
          y1={goalBottomY + dimensions.goal_depth}
          x2={(dimensions.field_width + dimensions.goal_width) / 2}
          y2={goalBottomY + dimensions.goal_depth}
          stroke="#8D00F2"
          strokeWidth={strokeWidth}
        />
        <line
          x1={(dimensions.field_width - dimensions.goal_width) / 2}
          y1={goalBottomY}
          x2={(dimensions.field_width - dimensions.goal_width) / 2}
          y2={goalBottomY + dimensions.goal_depth}
          stroke="#8D00F2"
          strokeWidth={strokeWidth}
        />
        <line
          x1={(dimensions.field_width + dimensions.goal_width) / 2}
          y1={goalBottomY}
          x2={(dimensions.field_width + dimensions.goal_width) / 2}
          y2={goalBottomY + dimensions.goal_depth}
          stroke="#8D00F2"
          strokeWidth={strokeWidth}
        />

        {/* Robôs azuis */}
        {blueRobots.map((robot) => {
          const adjustedOrientation = 90 - (robot.orientation ?? 0); // Ajusta conforme seu padrão
          return (
            <image
              key={`blue-${robot.id}`}
              href={`/img/blue_team/id${robot.id}.png`}
              x={robot.x - robotSize / 2}
              y={robot.y - robotSize / 2}
              width={robotSize}
              height={robotSize}
              transform={`rotate(${adjustedOrientation}, ${robot.x}, ${robot.y})`}
              pointerEvents="none"
            />
          );
        })}

        {yellowRobots.map((robot) => {
          // Inverter a orientação para espelhar no eixo vertical
          const adjustedOrientation = 90 - (robot.orientation ?? 0); // subtrai 90 para alinhar e inverte o ângulo

          return (
            <image
              key={`yellow-${robot.id}`}
              href={`/img/yellow_team/id${robot.id}.png`}
              x={robot.x - robotSize / 2}
              y={robot.y - robotSize / 2}
              width={robotSize}
              height={robotSize}
              transform={`rotate(${adjustedOrientation}, ${robot.x}, ${robot.y})`}
              pointerEvents="none"
            />
          );
        })}

        {/* Bola */}
        {ball && (
          <image
            href="/img/ball.png"
            x={ball.x - ballSize / 2}
            y={ball.y - ballSize / 2}
            width={ballSize}
            height={ballSize}
            pointerEvents="none"
          />
        )}
      </g>
    </svg>
  );
}
