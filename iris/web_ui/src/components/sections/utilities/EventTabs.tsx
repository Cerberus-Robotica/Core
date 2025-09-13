import { useState } from 'react';
import EventsBallLeftFieldTemplate from '../templates/EventsBallLeftFieldTemplate';
import EventsFoulsTemplate from '../templates/EventsFoulsTemplate';
import EventsGoalTemplate from '../templates/EventsGoalTempalte';
import EventsOthersTemplate from '../templates/EventsOthersTemplate';

export default function EventTabs() {
  const [activeTab, setActiveTab] = useState('ball_left');

  const tabs = [
    { id: 'ball_left', label: 'BALL LEFT FIELD' },
    { id: 'goal', label: 'GOAL' },
    { id: 'foul', label: 'FOUL' },
    { id: 'others', label: 'OTHERS' },
  ];

  return (
    <div className="w-full max-w-4xl mx-auto">
      {/* Cabeçalho das abas */}
      <div className="flex">
        {tabs.map((tab) => (
          <button
            key={tab.id}
            onClick={() => setActiveTab(tab.id)}
            className={`flex-1 px-4 py-4 text-sm font-medium border-b-2 bg-[#6d6d6d] ${
              activeTab === tab.id
                ? 'border-[#8D00F2] text-white bg-[#494949]'
                : 'border-transparent text-[#313131] hover:text-[#000000]'
            }`}
          >
            {tab.label}
          </button>
        ))}
      </div>

      {/* Conteúdo das abas */}
      <div className="p-4 bg-[#7e7e7e]">
        {activeTab === 'ball_left' && <EventsBallLeftFieldTemplate/>}
        {activeTab === 'goal' && <EventsGoalTemplate/>}
        {activeTab === 'foul' && <EventsFoulsTemplate/>}
        {activeTab === 'others' && <EventsOthersTemplate/>}
      </div>
    </div>
  );
}
