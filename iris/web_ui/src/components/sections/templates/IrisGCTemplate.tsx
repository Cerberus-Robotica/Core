import { useState } from "react";
import GoalkeeperIdInput from "../utilities/GoalkeeperIdInput";
import CurrentCommandButton from "../utilities/CurrentCommandButton";
import { sendPost } from "../../../hooks/useSendPost"; // função que você já usa para enviar POST
import EventTabs from "../utilities/EventTabs";

export default function IrisGCTemplate() {
  const [posX, setPosX] = useState(0);
  const [posY, setPosY] = useState(0);

  const applyPosition = async () => {
    try {
      await sendPost("http://localhost:5000/command", {
        designated_position_x: posX,
        designated_position_y: posY,
      });
      console.log(`Posição aplicada: X=${posX}, Y=${posY}`);
    } catch (err) {
      console.error("Erro ao aplicar posição:", err);
    }
  };

  return (
    <>
      <GoalkeeperIdInput />
      <h2 className="text-lg font-bold mb-4">Comandos:</h2>
      <div className="flex flex-col items-center space-y-1 mt-4">
        {/* Primeiros botões (um embaixo do outro) */}

        {/* Botões divididos em 2 colunas */}
        <div className="grid grid-cols-2 gap-2 w-3/4">
          <div>
            <CurrentCommandButton label="HALT" value={0} color="default" />
            <CurrentCommandButton label="STOP" value={1} color="default" />
          </div>
          <div>
            <CurrentCommandButton label="NORMAL START" value={2} color="default" />
            <CurrentCommandButton label="FORCE START" value={3} color="default" />
          </div>

          {/* Coluna Amarela */}
          <div>
            <CurrentCommandButton label="KICKOFF" value={4} color="yellow" />
            <CurrentCommandButton label="PENALTY" value={6} color="yellow" />
            <CurrentCommandButton label="DIRECT FREE KICK" value={8} color="yellow" />
            <CurrentCommandButton label="INDIRECT FREE KICK" value={10} color="yellow" />
            <CurrentCommandButton label="TIME OUT" value={12} color="yellow" />
            <CurrentCommandButton label="GOAL" value={14} color="yellow" />
            <CurrentCommandButton label="BALL PLACEMENT" value={16} color="yellow" />
          </div>

          {/* Coluna Azul */}
          <div>
            <CurrentCommandButton label="KICKOFF" value={5} color="blue" />
            <CurrentCommandButton label="PENALTY" value={7} color="blue" />
            <CurrentCommandButton label="DIRECT FREE KICK" value={9} color="blue" />
            <CurrentCommandButton label="INDIRECT FREE KICK" value={11} color="blue" />
            <CurrentCommandButton label="TIME OUT" value={13} color="blue" />
            <CurrentCommandButton label="GOAL" value={15} color="blue" />
            <CurrentCommandButton label="BALL PLACEMENT" value={17} color="blue" />
          </div>
        </div>

        {/* Inputs de posição */}
        <div className="flex flex-col items-center mt-4 space-y-2 w-3/4">

            <label className="mr-2 font-medium">Designar Posição X:</label>
          <div className="flex justify-between w-full">
            <input
              type="number"
              value={posX}
              onChange={(e) => setPosX(parseFloat(e.target.value))}
              className="border rounded px-2 py-1 w-full"
            />
          </div>
          <label className="mr-2 font-medium">Designar Posição Y:</label>
          <div className="flex justify-between w-full">
            <input
              type="number"
              value={posY}
              onChange={(e) => setPosY(parseFloat(e.target.value))}
              className="border rounded px-2 py-1 w-full"
            />
          </div>
          <button
            onClick={applyPosition}
            className="bg-green-600 text-white px-4 py-2 rounded hover:bg-green-700"
          >
            Aplicar Posição
          </button>
        </div>
      </div>
      <h2 className="text-lg font-bold mb-4">Eventos:</h2>
      <div className=" ">
          <EventTabs/>
        </div>
    </>
  );
}
