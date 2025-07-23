// src/components/sections/CaronteSection.tsx
import type { DataType } from '../../types';

type Props = {
  data: DataType;
};

export default function CaronteSection({ data }: Props) {
  return (
    <>
      <h2 className="text-lg font-bold mb-1">Caronte</h2>
      <p>
        Processo: <span className="font-mono">{data.caronte.processo}</span>
      </p>
      <p>
        Estratégia: <span className="font-mono">{data.caronte.estrategia}</span>
      </p>
    </>
  );
}
