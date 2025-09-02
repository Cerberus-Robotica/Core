import type { DataType } from '../../types';
import { ToggleSwitch } from './utilities/ToggleSwitch';

type Props = {
  data: DataType;
  toggleBoolean: (key: string, currentValue: boolean) => void;
};

export default function CompetitionSection({ data, toggleBoolean }: Props) {
  return (
    <div>
      <p>
        Modo Competição:{' '}
        <span className="font-mono">
          {data.tartarus.competition_mode ? 'Sim' : 'Não'}
        </span>
      </p>
      <ToggleSwitch
        value={data.tartarus.competition_mode}
        onToggle={() =>
          toggleBoolean('competition_mode', data.tartarus.competition_mode)
        }
      />
    </div>
  );
}
