import { useState } from "react";
import { MenuButton } from "./MenuButton";
import type { SoftwareOption } from "../App";

export function MenuBar({ onSelectSoftware }: { onSelectSoftware: (software: SoftwareOption) => void }) {
  const [active, setActive] = useState<string | null>(null);

  return (
    <nav className="w-full bg-[#3B3B3B] text-white flex items-center justify-between px-1 py-1 shadow-md relative">
      <div className="flex space-x-4 flex-1 max-w-xs relative">
        <MenuButton
          label="Dados"
          variant={active === "dados" ? "primary" : "default"}
          onClick={() => setActive(active === "dados" ? null : "dados")}
        />
        {active === "dados" && (
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
        <MenuButton label="Botão 2" variant="default" onClick={() => {}} />
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
