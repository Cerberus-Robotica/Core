import type { FIELD_DIMENSIONS } from "../data/fieldDimensions";
import { FieldSVG, type Robot } from "./FieldSVG";

type FieldDimensions = (typeof FIELD_DIMENSIONS)[keyof typeof FIELD_DIMENSIONS];

type Props = {
  dimensions: FieldDimensions;
  blueRobots?: Robot[];
  yellowRobots?: Robot[];
};

export function FieldView({ dimensions, blueRobots = [], yellowRobots = [] }: Props) {
  return (
    <div
      className="h-full flex justify-center items-center bg-[#3B3B3B]"
      style={{
        minWidth: "500px",
        maxWidth: "1000px",
        flexShrink: 0,
        padding: "16px",
      }}
    >
      <div
        className="w-full h-full flex justify-center items-center"
        style={{
          aspectRatio:
            dimensions.field_width /
            (dimensions.field_length + 2 * dimensions.goal_depth),
        }}
      >
        <FieldSVG
          dimensions={dimensions}
          blueRobots={blueRobots}
          yellowRobots={yellowRobots}
        />
      </div>
    </div>
  );
}
