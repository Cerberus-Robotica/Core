import { ActionButton } from './ActionButton';
import { RowWrapper } from './RowWrapper';

type NumberInputRowProps = {
  label: string;
  value: number;
  setValue: React.Dispatch<React.SetStateAction<number>>;
  onSubmit: () => void;
};

export function NumberInputRow({
  label,
  value,
  setValue,
  onSubmit,
}: NumberInputRowProps) {
  return (
    <RowWrapper>
      <p>{label}</p>
      <div className="flex">
        <input
          type="number"
          value={value}
          onChange={(e) => setValue(Number(e.target.value))}
          className="border rounded px-2 py-1 mr-2 w-24"
        />
        <ActionButton onClick={onSubmit} label="Alterar" />
      </div>
    </RowWrapper>
  );
}
