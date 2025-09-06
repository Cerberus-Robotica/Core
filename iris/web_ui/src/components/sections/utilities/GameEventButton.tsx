import { sendPost } from "../../../hooks/useSendPost";

type Props = {
  label: string;
  value: number;
  color: "yellow" | "blue" | "default";
};

export default function GameEventButton({ label, value, color }: Props) {
  const handleClick = async () => {
    // sempre reseta para 0 antes de enviar o novo evento
    await sendPost("http://localhost:5000/command", { game_event: 0 });
    await sendPost("http://localhost:5000/command", { game_event: value });
  };

  const bgColor =
    color === "yellow"
      ? "bg-yellow-500 hover:bg-yellow-600 text-black"
      : color === "blue"
      ? "bg-blue-600 hover:bg-blue-700 text-white"
      : "bg-gray-200 hover:bg-gray-300 text-black";

  return (
    <button
      onClick={handleClick}
      className={`w-full py-2 px-4 rounded-lg font-semibold mb-2 transition ${bgColor}`}
    >
      {label}
    </button>
  );
}
