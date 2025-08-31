type Props = { onClick: () => void; label: string };

export function ActionButton({ onClick, label }: Props) {
  return (
    <button
      onClick={onClick}
      className="w-[100px] px-3 py-1 bg-blue-600 hover:bg-blue-700 text-white rounded"
    >
      {label}
    </button>
  );
}
