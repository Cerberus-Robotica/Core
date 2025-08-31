type Props = {
  value: boolean;
  onToggle: () => void;
  disabled?: boolean; // 🔥 adiciona isso
};

export function ToggleSwitch({ value, onToggle }: Props) {
  return (
    <div
      onClick={onToggle}
      className={`w-12 h-6 flex items-center rounded-full p-1 cursor-pointer ${
        value ? 'bg-green-500' : 'bg-red-500'
      }`}
    >
      <div
        className={`bg-white w-4 h-4 rounded-full shadow-md transform duration-300 ${
          value ? 'translate-x-6' : 'translate-x-0'
        }`}
      />
    </div>
  );
}
