/*
 * SyncableManager.h
 *
 *  Created on:	28.01.2021
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_SYNCABLEMANAGER_H_
#define SRC_SYNCABLEMANAGER_H_

#include "ShortNames.h"

#include <map>
#include <SFML/Network.hpp>

#define NET_GAME_EVENT_INVALID 0
#define NET_GAME_EVENT_SPAWN 1
#define NET_GAME_EVENT_DELETE 2


class Syncable;
class NetGameEventListener;
class SyncableFactory;
class Simulation;
class IWorld;
class TickServiceProvider;

class SyncableManager
{
	/* send | recv	| use case
	 * 	0	|	0	| singleplayer	(currently unused)
	 * 	1	|	0	| server
	 * 	0	|	1	| client
	 * 	1	|	1	| relay			(currently unused)
	 */
	bool sender=false;
	bool receiver=false;

	syncID nextID=1;

	syncID updateQueryIndex=0;
	Syncable * getNextToUpdate();


	std::vector<sf::Packet> events;//TODO no doubles

	std::map<u32,NetGameEventListener *> listenerMap;

	SyncableFactory * factory;

	Simulation & sim;

	void addSyncable(Syncable * s);

	void removeSyncable(syncID ID);
	void removeSyncable(Syncable * s);

	void skipBytes(sf::Packet p,u32 bytes);

	void createSpawnEvent(Syncable * s);//just creates the event, does not spawn itself
	void createDeleteEvent(syncID sID);//just creates the event, does not delete itself


	void internalSpawn(Syncable * s);//performs required internal actions for spawn
	void internalDelete(Syncable * s);//performs required internal actions for delete

	bool hasEntry(u32 netGameEventID);


	syncID getNextID();//never call on client side!!!

public:
	void properDelete(syncID sID);//call this to delete
	void properSpawn(Syncable * s);//call this to spawn locally
	void properSpawn(sf::Packet& p,syncID sID);//call this to spawn from network

	std::map<syncID,Syncable *> syncMap;

	bool exists(syncID sID);

	void fillUpdatePacket(sf::Packet& p,u32 byteBudget);

	void fillCompletePacket(sf::Packet& p);
	void fetchEventPackets(sf::Packet& p);

	void applyUpdatePacket(sf::Packet& p);
	void applyEventPacket(sf::Packet& p);


	/* only one can ever be active for the same eventID at one time
	 * per SyncableManager (due to the way "sf::Packet" works).
	 * If there is already one registered, the old one will be deregistered.
	 */
	void registerNetGameEventListener(NetGameEventListener * l);
	void removeNetGameEventListener(NetGameEventListener * l);

	sf::Packet& createGenericEvent(u32 eventID);//returned Packet can be fed with custom event data

	SyncableManager(Simulation & s);
	virtual ~SyncableManager();

	IWorld & getIWorld();
	TickServiceProvider & getTSP();


	//move and copy protection
	SyncableManager(const SyncableManager&) = delete;
	SyncableManager(SyncableManager&&) = delete;
	SyncableManager& operator=(const SyncableManager&) = delete;
	SyncableManager& operator=(SyncableManager&&) = delete;
};

#endif /* SRC_SYNCABLEMANAGER_H_ */
