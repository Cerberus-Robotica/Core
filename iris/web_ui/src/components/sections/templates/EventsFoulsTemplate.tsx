import GameEventButton from '../utilities/GameEventButton';

export default function EventsFoulsTemplate() {
  return (
    <>
      <p>Stopping Fouls:</p>
      <div className="mt-3 mb-3 grid grid-cols-3 gap-1">
        <div>
          <GameEventButton
            label="ATK CLOSE DEF AREA"
            value={19}
            color="default"
          />
          <GameEventButton label="DEF IN AREA" value={31} color="default" />
          <GameEventButton label="BOUNDARY" value={41} color="default" />
        </div>
        <div>
          <GameEventButton label="KEEPER HOLD" value={13} color="default" />
          <GameEventButton label="DRIBBLE FAR" value={17} color="default" />
          <GameEventButton label="PUSH" value={24} color="default" />
        </div>
        <div>
          <GameEventButton label="HOLD BALL" value={26} color="default" />
          <GameEventButton label="TIPPED" value={27} color="default" />
          <GameEventButton label="DROP PARTS" value={47} color="default" />
        </div>
      </div>

      <p>Non-Stopping Fouls:</p>
      <div className="mt-3 mb-3 grid grid-cols-2 gap-1">
        <div>
          <GameEventButton
            label="ATK TOUCH IN DEF AREA"
            value={15}
            color="default"
          />
          <GameEventButton label="KICK TOO FAST" value={18} color="default" />
        </div>
        <div>
          <GameEventButton label="CRASH UNIQUE" value={22} color="default" />
          <GameEventButton label="CRASH DRAWN" value={21} color="default" />
        </div>
      </div>

      <p>Ball Out Fouls:</p>
      <div className="mt-3 mb-3 grid grid-cols-2 gap-1">
        <div>
          <GameEventButton
            label="DEF TOO CLOSE TO KICK"
            value={29}
            color="default"
          />
          <GameEventButton label="TOO FAST IN STOP" value={28} color="default" />
        </div>
        <div>
          <GameEventButton label="PLACEMENT INTERF." value={20} color="default" />
          <GameEventButton label="EXCESS SUBSTITUTION" value={48} color="default" />
        </div>
      </div>
    </>
  );
}
