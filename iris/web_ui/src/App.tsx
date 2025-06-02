import DataView from "./components/DataView";
import FieldView from "./components/FieldView";
import MenuBar from "./components/TopMenuBar";

function App() {
  return (
    <>
      <div className="bg-[#311A52] h-screen overflow-hidden">
        <MenuBar />
        <div className="flex ">
          <FieldView />
          <DataView />
        </div>
      </div>
    </>
  );
}

export default App;
