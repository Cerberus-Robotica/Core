import React, { useEffect, useState, useRef } from "react";
import axios from "axios";

// Interface para definir a estrutura dos dados esperados
interface DataType {
  caronte: {
    processo: string;
    estrategia: string;
  };
  ia: {
    robots_size: number;
  };
  vision: {
    timestamp: number;
    field_length: number;
  };
  gc: {
    team_blue: boolean;
    score_yellow: number;
    score_blue: number;
  };
  tartarus: {
    ssl_vision: boolean;
    team_blue_status: boolean;
  };
}

const DataView: React.FC = () => {
  // Inicialização do estado 'data' com valores padrão que correspondem à interface
  const [data, setData] = useState<DataType>({
    caronte: { processo: "Aguardando dados...", estrategia: "Aguardando dados..." },
    ia: { robots_size: 0 },
    vision: { timestamp: 0, field_length: 0 },
    gc: { team_blue: false, score_yellow: 0, score_blue: 0 },
    tartarus: { ssl_vision: false, team_blue_status: false },
  });
  const [reading, setReading] = useState<boolean>(false);
  const intervalRef = useRef<number | null>(null);

  useEffect(() => {
    // Se 'reading' for true, inicia o intervalo para buscar dados
    if (reading) {
      // Limpa qualquer intervalo existente antes de iniciar um novo
      if (intervalRef.current) {
        clearInterval(intervalRef.current);
      }
      intervalRef.current = setInterval(fetchData, 500) as unknown as number; // Adicione 'as unknown as number' para tipagem correta no TS
    } else if (intervalRef.current) {
      // Se 'reading' for false e um intervalo existir, limpa-o
      clearInterval(intervalRef.current);
      intervalRef.current = null;
    }

    // Função de limpeza para garantir que o intervalo seja limpo quando o componente for desmontado
    return () => {
      if (intervalRef.current !== null) {
        clearInterval(intervalRef.current);
      }
    };
  }, [reading]); // O efeito é re-executado quando 'reading' muda

  const fetchData = async () => {
    try {
      const response = await axios.get<DataType>("http://localhost:5000/data"); // Tipa a resposta do axios
      setData(response.data);
    } catch (error) {
      // Em caso de erro, você pode resetar para valores padrão ou manter os últimos conhecidos
      // Para evitar que a tela fique em branco em caso de erro, você pode não alterar o estado
      // ou definir um estado de erro visível.
      console.error("Erro ao buscar dados:", error);
      // Opcional: setData(initialData) ou setData(null) e ajustar a renderização
    }
  };

  return (
    <div className="p-2 w-full md:w-[300px] lg:w-[350px]">
      <div className="p-4 bg-[#545454] text-white border-[#6805F2] border-3 rounded-[5px] w-full">
        <button
          onClick={() => setReading(!reading)}
          className={`mb-4 w-full py-2 rounded-[5px] font-semibold transition-colors duration-200 ${
            reading ? "bg-red-600 hover:bg-red-700" : "bg-green-600 hover:bg-green-700"
          } text-white`}
        >
          {reading ? "Parar leitura" : "Iniciar leitura"}
        </button>

        {/* Informações da Caronte */}
        <h2 className="text-lg font-bold mb-1">Caronte</h2>
        <p>Processo: <span className="font-mono">{data.caronte.processo}</span></p> {/* Acessando diretamente pois a inicialização garante a estrutura */}
        <p>Estratégia: <span className="font-mono">{data.caronte.estrategia}</span></p>

        <div className="my-2 border-t border-[#3B3B3B]"></div>

        {/* Informações da IA */}
        <h2 className="text-lg font-bold mb-1">IA</h2>
        <p>Robots Size: <span className="font-mono">{data.ia.robots_size}</span></p>

        <div className="my-2 border-t border-[#3B3B3B]"></div>

        {/* Informações da Vision */}
        <h2 className="text-lg font-bold mb-1">Vision</h2>
        <p>Timestamp: <span className="font-mono">{data.vision.timestamp}</span></p>
        <p>Field Length: <span className="font-mono">{data.vision.field_length}</span></p>

        <div className="my-2 border-t border-[#3B3B3B]"></div>

        {/* Informações do Game Controller (GC) */}
        <h2 className="text-lg font-bold mb-1">Game Controller (GC)</h2>
        <p>Team Blue: <span className="font-mono">{data.gc.team_blue ? "Sim" : "Não"}</span></p>
        <p>Score Yellow: <span className="font-mono">{data.gc.score_yellow}</span></p>
        <p>Score Blue: <span className="font-mono">{data.gc.score_blue}</span></p>

        <div className="my-4 border-t border-[#3B3B3B]"></div>

        {/* Informações do Tartarus */}
        <h2 className="text-lg font-bold mb-1">Tartarus</h2>
        <p>SSL Vision: <span className="font-mono">{data.tartarus.ssl_vision ? "Sim" : "Não"}</span></p>
        <p>
          Team Blue Status: <span className="font-mono">{data.tartarus.team_blue_status ? "Sim" : "Não"}</span>
        </p>
      </div>
    </div>
  );
};

export default DataView;