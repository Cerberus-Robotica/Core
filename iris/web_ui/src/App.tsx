import { useState } from 'react';
import { DataView } from './components/DataView';
import { FieldView } from './components/FieldView';
import { MenuBar } from './components/TopMenuBar';
import { DataViewAll } from './components/DataViewAll';

export type SoftwareOption = 'ia' | 'gc' | 'vision' | 'tartarus' | 'caronte';

export default function App() {
  const [selectedSoftware, setSelectedSoftware] =
    useState<SoftwareOption | null>(null);
  const [reading, setReading] = useState(false); // 🔁 Controle global da leitura

  return (
    <div className="bg-[#311A52] h-screen w-screen overflow-hidden">
      <div className="flex flex-col h-screen w-screen">
        <div className="h-[35px]">
          <MenuBar onSelectSoftware={setSelectedSoftware} />
        </div>

        <div className="flex w-full h-full overflow-hidden">
          {/* Campo com largura automática baseada na altura */}
          <FieldView />

          {/* Painel de dados ocupa o restante da tela */}
          <div className="flex flex-1 h-full overflow-hidden">
            <DataView reading={reading} setReading={setReading} />
            {selectedSoftware && (
              <DataViewAll reading={reading} selected={selectedSoftware} />
            )}
          </div>
        </div>
      </div>
    </div>
  );
}
