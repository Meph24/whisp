#include "EventMapper.hpp"
#include <iostream>
#include "SFML/Window.hpp"

EventMapper::EventMapper(ControlInputStatusSet& control_input_status_set)
	: managed_stati(&control_input_status_set)
	, pm(1)
{
	std::cout << "managed_stati[0]=" << (*managed_stati)[0] << " managed_stati[1]=" << (*managed_stati)[1] << std::endl;
	std::cout << "managed_stati adress: " << managed_stati->data() << std::endl;
	std::cout << "managed_stati size=" << managed_stati->size() << std::endl;
}

void EventMapper::event(EventHandler::event& e)
{
	ControlInputStatusSet& stati = *managed_stati;

	for(EventMapping& m : event_id_event_mappings[e.ID])
	{
		m.map(e, *managed_stati);
	}

	for(mapping& myM : event_id_mappings[e.ID])
	{
		bool evAsTrue=false;
		if(myM.condition==0)
			evAsTrue=true;
		else if(myM.condition>=(int)stati.size()||myM.condition<=-(int)stati.size())
		{
			WarnErrReporter::invalidValueErr("Event handling condition points to outside of bounds index, ignoring this event...");
			continue;
		}
		if(myM.condition>0&&stati[myM.condition]==0) evAsTrue=true;
		else if(myM.condition<0&&stati[-myM.condition]!=0) evAsTrue=true;
		if(!evAsTrue) continue;
		if(myM.mode==MAPPER_MODE_EMPTY) (void)0;//do nothing
		else if(myM.mode==MAPPER_MODE_HOLD)
		{
			if(e.value>0)
				stati[myM.statusIndex]+=myM.mapParam;//TODO multi-stage mapping
			else
				stati[myM.statusIndex]-=myM.mapParam;
		}
		else if(myM.mode==MAPPER_MODE_TOGGLE)
		{
			if(e.value==myM.mapParam)
			{
				int indx=myM.statusIndex;
				std::cout<<"toggle "<<indx<<std::endl;
				if(stati[indx]==0) stati[indx]=1;//toggle between 0 and 1
				else stati[indx]=0;
			}
		}
		else if(myM.mode==MAPPER_MODE_ADD)
		{
			stati[myM.statusIndex]+=e.value+myM.mapParam;
		}
		else if(myM.mode==MAPPER_MODE_ADD_RESETM)
		{
			stati[myM.statusIndex]+=e.value+myM.mapParam;
			if(e.ID==2048)//mouse x
			{
				sf::Vector2i mousePos=sf::Mouse::getPosition();
				mousePos.x=-myM.mapParam;
				sf::Mouse::setPosition(mousePos);
			}
			else if(e.ID==2049)//mouse y
			{
				sf::Vector2i mousePos=sf::Mouse::getPosition();
				mousePos.y=-myM.mapParam;
				sf::Mouse::setPosition(mousePos);
			}
		}
		else if(myM.mode==MAPPER_MODE_REPLACE)
		{
			stati[myM.statusIndex]=e.value+myM.mapParam;
		}

		std::cout << "InputEvent::id" << e.ID << " stati[myM.statusIndex]=" << stati[myM.statusIndex] << std::endl;
	}
}

void EventMapper::registerAction(int id, int mode, int condition,int statusIndex, float mapParam)
{
		event_id_mappings[id].push_back({mode,condition,statusIndex,mapParam});
}

void EventMapper::registerMapping(int eventid, EventMapping mapping)
{	
	event_id_event_mappings[eventid].emplace_back(mapping);
}

void EventMapper::registerMapping(int eventid, EventMappingAction action, EventMappingCondition condition)
{
	registerMapping(eventid, EventMapping(action, condition));	
}

void EventMapper::clearAllMappings(){ event_id_mappings.clear(); event_id_event_mappings.clear(); }
