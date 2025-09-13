import GameEventButton from '../utilities/GameEventButton';

export default function EventsOthersTemplate() {
  return (
    <>
      <div className="mt-3 mb-3 grid grid-cols-2 gap-1">
        <div>
          <GameEventButton
            label="ATTACKER DOUBLE TOUCHED BALL"
            value={14}
            color="default"
          />
          <GameEventButton
            label="PLACEMENT SUCCEEDED"
            value={5}
            color="default"
          />
          <GameEventButton
            label="PENALTY KICK FAILED"
            value={43}
            color="default"
          />
          <GameEventButton
            label="NO PROGRESS IN GAME"
            value={2}
            color="default"
          />
          <GameEventButton label="PLACEMENT FAILED" value={3} color="default" />
          <GameEventButton label="MULTIPLE CARDS" value={32} color="default" />
          <GameEventButton label="MULTIPLE FOULS" value={34} color="default" />
        </div>
        <div>
          <GameEventButton
            label="BOT SUBSTITUTION"
            value={37}
            color="default"
          />
          <GameEventButton label="TOO MANY ROBOTS" value={38} color="default" />
          <GameEventButton label="CHALLENGE FLAG" value={44} color="default" />
          <GameEventButton
            label="CHALLENGE FLAG HANDLED"
            value={46}
            color="default"
          />
          <GameEventButton label="EMERGENCY STOP" value={45} color="default" />
          <GameEventButton
            label="UNSPORTING BEHAVIOR (MINOR)"
            value={35}
            color="default"
          />
          <GameEventButton
            label="UNSPORTING BEHAVIOR (MAJOR)"
            value={36}
            color="default"
          />
        </div>
      </div>
    </>
  );
}
