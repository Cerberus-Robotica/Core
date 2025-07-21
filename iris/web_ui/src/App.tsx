import DataView from "./components/DataView";
import FieldView from "./components/FieldView";
import MenuBar from "./components/TopMenuBar";

function App() {
  return (
    <>
      <div className="bg-[#311A52] h-screen overflow-hidden">
        <MenuBar />
        <div className="flex flex-col lg:flex-row lg:justify-center gap-4 p-4">
          <FieldView />
          <DataView />
        </div>
      </div>
    </>
  );
}

export default App;
