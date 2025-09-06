import GameEventButton from '../utilities/GameEventButton';

export default function EventsBallLeftFieldTemplate() {
  return (
    <>
      <div className='grid grid-cols-3 gap-1'>
        <GameEventButton label="TOUCH LINE" value={6} color="default" />
        <GameEventButton label="GOAL LINE" value={7} color="default" />
        <GameEventButton label="AIMLESS KICK" value={11} color="default" />
      </div>
    </>
  );
}
