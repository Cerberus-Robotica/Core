// src/components/sections/GCSection.tsx

import type { DataType } from "../../../types";
import GoalkeeperIdInput from "../utilities/GoalkeeperIdInput";

type Props = {
  data: DataType;
};

export default function IrisGCTemplate({ data }: Props) {
  return (
    <>
      <GoalkeeperIdInput/>
    </>
  );
}
