type MenuButtonProps = {
  label: string;
  onClick?: () => void;
  variant?: 'primary' | 'default';
};

export default function MenuButton({ label, onClick, variant = 'default' }: MenuButtonProps) {
  const baseStyle = 'flex-1 py-0.5 rounded-sm transition';
  const variants = {
    primary: 'bg-[#3D1870] hover:bg-[#311A52]',
    default: 'bg-[#6805F2] hover:bg-[#34007D]',
  };

  return (
    <button onClick={onClick} className={`${baseStyle} ${variants[variant]}`}>
      {label}
    </button>
  );
}
