type Props = {
  imageSrc: string;
};

export function CompetitionOverlay({ imageSrc }: Props) {
  return (
    <div className="absolute inset-0 -m-4 flex flex-col items-center justify-center z-50 bg-black/70 pointer-events-auto">
      <h1 className="text-white text-2xl font-bold mb-4 text-center">
        Modo competição ativado
      </h1>
      <img
        src={imageSrc}
        alt="Competição"
        className="max-w-[40%] max-h-[40%]"
      />
    </div>
  );
}
