import { sendPost } from "../../../hooks/useSendPost";

type Props = {
  label: string;
  value: number;
  color: "default";
};

export default function GameEventButton({ label, value, color }: Props) {
  const handleClick = async () => {
    // sempre reseta para 0 antes de enviar o novo evento
    await sendPost("http://localhost:5000/command", { game_event: 0 });
    await sendPost("http://localhost:5000/command", { game_event: value });
  };

  const bgColor =
    color === "default"
      ? "bg-white hover:bg-[#acacac] text-black"
      : "bg-white hover:bg-[#acacac] text-black";

  return (
    <button
      onClick={handleClick}
      className={`w-full py-2 px-4 rounded-lg font-semibold mb-2 transition ${bgColor}`}
    >
      {label}
    </button>
  );
}
