import { useState } from 'react';
import { DataView } from './components/DataView';
import { FieldView } from './components/FieldView';
import { MenuBar } from './components/TopMenuBar';
import { DataViewAll } from './components/DataViewAll';

export type SoftwareOption = 'ia' | 'gc' | 'vision' | 'tartarus' | 'caronte';

export default function App() {
  const [selectedSoftware, setSelectedSoftware] = useState<SoftwareOption | null>(null);
  const [reading, setReading] = useState(false); // 🔁 Controle global da leitura

  return (
    <div className="bg-[#311A52] h-screen overflow-hidden">
      <MenuBar onSelectSoftware={setSelectedSoftware} />
      <div className="flex flex-col lg:flex-row lg:justify-center gap-4 p-4">
        <FieldView />
        <DataView reading={reading} setReading={setReading} /> {/* passa controle */}
        {selectedSoftware && <DataViewAll reading={reading} selected={selectedSoftware} />}
      </div>
    </div>
  );
}
