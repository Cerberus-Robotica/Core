import { useState } from 'react';
import { MenuButton } from './MenuButton';
import type { SoftwareOption } from '../App';
import type { Division } from '../data/fieldDimensions';

type Props = {
  onSelectSoftware: (software: SoftwareOption) => void;
  division: Division;
  setDivision: (div: Division) => void;
};

export function MenuBar({ onSelectSoftware, division, setDivision }: Props) {
  const [active, setActive] = useState<string | null>(null);

  return (
    <nav className="w-full bg-[#3B3B3B] text-white flex items-center justify-between px-1 py-1 shadow-md relative">
      <div className="flex space-x-4 flex-1 max-w-xs relative">
        {/* Botão Dados */}
        <MenuButton
          label="Dados"
          variant={active === 'dados' ? 'primary' : 'default'}
          onClick={() => setActive(active === 'dados' ? null : 'dados')}
        />
        {active === 'dados' && (
          <div className="absolute top-full z-10 bg-[#3B3B3B] rounded border border-gray-600">
            <select
              className="bg-[#3B3B3B] text-white p-2 w-full"
              onChange={(e) => onSelectSoftware(e.target.value as SoftwareOption)}
              defaultValue=""
            >
              <option disabled value="">
                Escolha um software
              </option>
              <option value="ia">IA</option>
              <option value="gc">Game Controller</option>
              <option value="vision">Vision</option>
              <option value="tartarus">Tartarus</option>
              <option value="caronte">AutoRef</option>
            </select>
          </div>
        )}

        {/* Botão Divisão */}
        <MenuButton
          label="Divisão"
          variant={active === 'divisao' ? 'primary' : 'default'}
          onClick={() => setActive(active === 'divisao' ? null : 'divisao')}
        />
        {active === 'divisao' && (
          <div className="absolute top-full left-[120px] z-10 bg-[#3B3B3B] rounded border border-gray-600">
            <select
              className="bg-[#3B3B3B] text-white p-2 w-full"
              value={division}
              onChange={(e) => setDivision(e.target.value as Division)}
            >
              <option value="Entry Level">Entry Level</option>
              <option value="Regular B">Regular B</option>
              <option value="Regular A">Regular A</option>
            </select>
          </div>
        )}
      </div>

      <div className="flex-1 flex justify-center">
        <img src="/img/LogoTextCerberus.png" className="h-8 w-auto select-none" />
      </div>

      <div className="flex space-x-4 flex-1 max-w-xs justify-end">
        <MenuButton label="Botão 3" variant="default" onClick={() => {}} />
        <MenuButton label="Botão 4" variant="default" onClick={() => {}} />
      </div>
    </nav>
  );
}
