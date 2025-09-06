// src/components/sections/GCSection.tsx

import type { DataType } from "../../../types";

type Props = {
  data: DataType;
};

export default function SslGCTemplate({ data }: Props) {
  return (
    <>
      <h2 className="text-lg font-bold mb-1">Game Controller</h2>
      <p>Team Blue: {data.gc.team_blue ? 'Sim' : 'Não'}</p>
      <p>Designated Position X: {data.gc.designated_position_x}</p>
      <p>Designated Position Y: {data.gc.designated_position_y}</p>
      <p>Current Command: {data.gc.current_command}</p>
      <p>Game Event: {data.gc.game_event}</p>

      <h2 className="font-bold mt-4">Time Azul</h2>
      <p>Nome: {data.gc.blue.name}</p>
      <p>Goleiro ID: {data.gc.blue.goalkeeper_id}</p>
      <p>Faltas: {data.gc.blue.fouls}</p>
      <p>Pontos: {data.gc.blue.score}</p>

      <h2 className="font-bold mt-4">Time Amarelo</h2>
      <p>Nome: {data.gc.yellow.name}</p>
      <p>Goleiro ID: {data.gc.yellow.goalkeeper_id}</p>
      <p>Faltas: {data.gc.yellow.fouls}</p>
      <p>Pontos: {data.gc.yellow.score}</p>
    </>
  );
}
