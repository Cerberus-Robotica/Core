import { useState, useEffect, useRef, type ReactNode } from 'react';

type MenuButtonProps = {
  label: string;
  onClick?: () => void;
  variant?: 'primary' | 'default';
  children?: ReactNode;             // conteúdo dropdown opcional
};

export function MenuButton({ label, onClick, variant = 'default', children }: MenuButtonProps) {
  const [open, setOpen] = useState(false);
  const buttonRef = useRef<HTMLButtonElement>(null);

  // Fecha o dropdown se clicar fora
  useEffect(() => {
    function handleClickOutside(event: MouseEvent) {
      if (buttonRef.current && !buttonRef.current.contains(event.target as Node)) {
        setOpen(false);
      }
    }
    if (open) {
      document.addEventListener('mousedown', handleClickOutside);
    } else {
      document.removeEventListener('mousedown', handleClickOutside);
    }
    return () => {
      document.removeEventListener('mousedown', handleClickOutside);
    };
  }, [open]);

  const baseStyle = 'flex-1 py-0.5 w-35 px-5 text-sm rounded-sm transition relative';

  const variants = {
    primary: 'bg-[#3D1870] hover:bg-[#311A52]',
    default: 'bg-[#6805F2] hover:bg-[#34007D]',
  };

  return (
    <div className="relative">
      <button
        ref={buttonRef}
        onClick={() => {
          if (children) {
            setOpen(!open);
          }
          if (onClick) {
            onClick();
          }
        }}
        className={`${baseStyle} ${variants[variant]}`}
        type="button"
      >
        {label}
      </button>
      {open && children && (
        <div className="absolute top-full left-0 mt-1 bg-[#3B3B3B] rounded border border-gray-600 shadow-lg z-20 min-w-max">
          {children}
        </div>
      )}
    </div>
  );
}
