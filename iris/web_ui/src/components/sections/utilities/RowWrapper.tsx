type Props = { children: React.ReactNode };

export function RowWrapper({ children }: Props) {
  return <div className="flex items-center justify-between mb-2">{children}</div>;
}
