import { sendPost } from '../hooks/useSendPost';

function TeamSelector() {
  const handleChangeTeam = async (isBlue: boolean) => {
    const success = await sendPost('http://localhost:5000/command', {
      team_blue: isBlue,
    });
    alert(success ? 'Time alterado' : 'Erro ao alterar time');
  };

  return (
    <div className="mt-4 space-x-2">
      <button
        onClick={() => handleChangeTeam(false)}
        className="bg-yellow-400 hover:bg-yellow-500 text-black font-bold py-2 px-4 rounded"
      >
        Time Amarelo
      </button>
      <button
        onClick={() => handleChangeTeam(true)}
        className="bg-blue-600 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded"
      >
        Time Azul
      </button>
    </div>
  );
}

export default TeamSelector;
