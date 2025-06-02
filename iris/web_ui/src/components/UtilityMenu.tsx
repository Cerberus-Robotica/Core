type UtilityMenuProps = {
  onSelectTerminal: () => void;
};

export default function UtilityMenu({ onSelectTerminal }: UtilityMenuProps) {
  return (
    <div className="absolute mt-2 w-40 bg-[#2f2f2f] rounded shadow-lg p-2 text-white z-50">
      <button
        onClick={onSelectTerminal}
        className="w-full text-left px-2 py-1 hover:bg-[#444] rounded"
      >
        Abrir Terminal
      </button>
    </div>
  );
}
