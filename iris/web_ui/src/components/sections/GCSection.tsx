// src/components/sections/GCSection.tsx
import type { DataType } from '../../types';
import { toggleBoolean } from '../../utils';
import IrisGCTemplate from './templates/IrisGCTemplate';
import SslGCTemplate from './templates/SslGCTemplate';
import { ToggleSwitch } from './utilities/ToggleSwitch';
type Props = {
  data: DataType;
};

export default function GCSection({ data }: Props) {
  const { tartarus } = data;
  return (
    <>
      <div className="flex flex-col items-center mb-4">
        <p className="mb-2">
          Game Controller:{' '}
          <span className="font-mono">
            {tartarus.iris_as_GC ? 'Iris' : 'SSL'}
          </span>
        </p>
        <ToggleSwitch
          value={tartarus.iris_as_GC}
          onToggle={() =>
            toggleBoolean('iris_as_GC', tartarus.iris_as_GC)
          }
        />
      </div>
      {tartarus.iris_as_GC ? (
        <IrisGCTemplate />
      ) : (
        <SslGCTemplate data={data} />
      )}
    </>
  );
}
