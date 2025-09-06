import { useState } from 'react';
import { sendPost } from '../../../hooks/useSendPost';
import { NumberInputRow } from './NumberInputRow';


export default function GoalkeeperIdInput() {
  const [goalieInput, setGoalieInput] = useState<number>(0);
  return (
    <NumberInputRow
      label="ID do Goleiro:"
      value={goalieInput}
      setValue={setGoalieInput}
      onSubmit={async () => {
        await sendPost('http://localhost:5000/command', {
          goalkeeper_id: goalieInput,
          goalkeeper_from_lcm: false, // essencial para impedir sobrescrita
        });
      }}
    />
  );
}
