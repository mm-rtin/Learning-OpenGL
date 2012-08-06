#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <map>
#include <vector>

#include <system/GameEvent.hpp>
#include <system/Event.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
              EventListener Interfaces
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** InputEventListener
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
class InputEventListener  {

public:
	InputEventListener();
	virtual ~InputEventListener();
	virtual bool onInputEvent(sf::Event&) = 0;
};

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** GameEventListener
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
class GameEventListener {

public:
	GameEventListener();
	virtual ~GameEventListener();
	virtual bool onGameEvent(GameEvent&) = 0;
};

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                      EventManager
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

// Define ListenerMap alias to multimap with integer key and class pointer IEventListener
// Int key is timestamp + priority int
typedef std::map<const int, InputEventListener*> InputListenerMap;
typedef std::map<const int, GameEventListener*> GameListenerMap;

/** inputListeners
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
struct InputListener {
	int key;
	sf::Event::EventType eventType;
};

/** gameListeners
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
struct GameListener {
	int key;
	GameEvent::eventTypes eventType;
};

class EventManager {

private:
	/// Array of pointers to ListenerMap
	InputListenerMap* inputEventList[sf::Event::Count];    /// input list
	GameListenerMap* gameEventList[GameEvent::COUNT];      /// game event list

	// event listeners to be deleted queue
	std::vector<InputListener*> inputListenersDeletionList;
	std::vector<GameListener*> gameListenersDeletionList;

	// methods
	void runInputDeletionList();
	void runGameDeletionList();

public:
	/// Constructor/Destructor
	EventManager();
	~EventManager();

	/// input events
	bool addEventListener(InputEventListener&, sf::Event::EventType);
	void removeEventListener(InputEventListener&, sf::Event::EventType);
	InputListenerMap* getListeners(sf::Event::EventType);
	void dispatchEvent(sf::Event&);

	/// game events
	bool addEventListener(GameEventListener&, GameEvent::eventTypes);
	void removeEventListener(GameEventListener&, GameEvent::eventTypes);
	GameListenerMap* getListeners(GameEvent::eventTypes);
	void dispatchEvent(GameEvent&);

};
#endif