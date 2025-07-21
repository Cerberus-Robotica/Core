import { useState } from "react";
import axios from 'axios';
import MenuButton from "./MenuButton";
import UtilityMenu from "./UtilityMenu";

export default function MenuBar() {
  const [active, setActive] = useState<string | null>(null);
  const [showUtilityMenu, setShowUtilityMenu] = useState(false);

  const handleUtilityClick = () => {
    setShowUtilityMenu((prev) => !prev);
    setActive("utility");
  };

  const handleTerminalSelect = () => {
    axios
      .post("http://localhost:4000/open-terminal")
      .then(() => console.log("Terminal aberto"))
      .catch((err) => console.error("Erro ao abrir terminal:", err));
  };

  return (
    <>
      <nav className="w-full bg-[#3B3B3B] text-white flex items-center justify-between px-1 py-1 shadow-md relative">
        <div className="flex flex-wrap gap-2 flex-1 relative">
          <MenuButton
            label="Utilidades"
            variant={active === "utility" ? "primary" : "default"}
            onClick={handleUtilityClick}
          />
          <MenuButton
            label="Botão 2"
            variant={active === "botao2" ? "primary" : "default"}
            onClick={() => setActive("botao2")}
          />
          {showUtilityMenu && (
            <div className="absolute top-full left-0">
              <UtilityMenu onSelectTerminal={handleTerminalSelect} />
            </div>
          )}
        </div>

        <div className="flex-1 flex justify-center">
          <img
            src="/img/LogoCerberus.png"
            className="h-10 w-auto select-none"
          />
        </div>

        <div className="flex flex-wrap gap-2 flex-1 relative">
          <MenuButton
            label="NomeExemplo3"
            variant={active === "botao3" ? "primary" : "default"}
            onClick={() => setActive("botao3")}
          />
          <MenuButton
            label="Botão 4"
            variant={active === "botao4" ? "primary" : "default"}
            onClick={() => setActive("botao4")}
          />
        </div>
      </nav>
    </>
  );
}
