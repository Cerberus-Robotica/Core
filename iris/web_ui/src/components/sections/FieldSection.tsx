import type { DataType } from '../../types';

type Props = {
  data: DataType;
};

// Wrapper padrão de linha
function RowWrapper({ children }: { children: React.ReactNode }) {
  return (
    <div className="mb-4 flex items-center justify-between">
      {children}
    </div>
  );
}

function ActionButton({
  onClick,
  label,
}: {
  onClick: () => void;
  label: string;
}) {
  return (
    <button
      onClick={onClick}
      className="w-[100px] px-3 py-1 bg-blue-600 hover:bg-blue-700 text-white rounded"
    >
      {label}
    </button>
  );
}

export default function FieldSection({
  data,
}: Props) {

  return (
    <>
      <h2 className="text-lg font-bold mb-4">Campo</h2>

      <RowWrapper>
        <p>
          Lado do Campo: <span className="font-mono"></span>
        </p>
        <ActionButton
          onClick={() => {}}
          label="Mudar Lado"
        />
      </RowWrapper>

      <h3 className="text-md font-semibold mt-2">Campo</h3>
      {data.vision.field ? (
        Object.entries(data.vision.field).map(([key, value]) => (
          <p key={key}>
            {key.replace(/_/g, ' ').replace(/^\w/, (c) => c.toUpperCase())}:{' '}
            <span className="font-mono">{value}</span>
          </p>
        ))
      ) : (
        <p className="italic text-gray-500">Dados de campo indisponíveis.</p>
      )}

    </>
  );
}
