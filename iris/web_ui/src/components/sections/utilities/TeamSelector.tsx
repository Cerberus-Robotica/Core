import { sendPost } from '../../../hooks/useSendPost';

function TeamSelector() {
  const handleChangeTeam = async (isBlue: boolean) => {
    const success = await sendPost('http://localhost:5000/command', {
      team_blue: isBlue,
    });
    alert(success ? 'Time alterado' : 'Erro ao alterar time');
  };

  return (
    <div className="mt-4 flex justify-center items-center gap-2">
        <button
            onClick={() => handleChangeTeam(false)}
            className="w-32 bg-yellow-500 hover:bg-yellow-600 text-black font-bold py-2 px-4 rounded"
        >
            Time Amarelo
        </button>
        <button
            onClick={() => handleChangeTeam(true)}
            className="w-32 bg-blue-600 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded"
        >
            Time Azul
        </button>
    </div>

  );
}

export default TeamSelector;
