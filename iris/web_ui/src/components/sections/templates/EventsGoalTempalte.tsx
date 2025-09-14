import GameEventButton from '../utilities/GameEventButton';

export default function EventsGoalTemplate() {
  return (
    <>
      <div className='grid grid-cols-3 gap-1'>
        <GameEventButton label="POSSIBLE GOAL" value={39} color="default" />
        <GameEventButton label="GOAL" value={8} color="default" />
        <GameEventButton label="INVALID GOAL" value={42} color="default" />
      </div>
    </>
  );
}
