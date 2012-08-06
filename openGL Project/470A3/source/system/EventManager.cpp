/*
     File:	EventManager.cpp

  Authors:	Michael Martin
  Created:	July 22, 2010

  Purpose:	
	 Note:
*/

#include <Windows.h>
#include <system/EventManager.hpp>

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
              EventListener Interfaces
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** InputEventListener
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
InputEventListener::InputEventListener() {}
InputEventListener::~InputEventListener() {}
bool InputEventListener::onInputEvent(sf::Event& inputEvent) {
	return true;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** GameEventListener
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
GameEventListener::GameEventListener() {}
GameEventListener::~GameEventListener() {}
bool GameEventListener::onGameEvent(GameEvent& gameEvent) {
	return true;
}

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                      EventManager
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/
EventManager::EventManager() {

	/// create ListenerMaps for each index of inputEventList
	int i;
	for (i = 0; i < sf::Event::Count; ++i) {
		inputEventList[i] = new InputListenerMap();
	}
	/// create ListenerMaps for each index of gameEventList
	int j;
	for (j = 0; j < GameEvent::COUNT; ++j) {
		gameEventList[j] = new GameListenerMap();
	}
};
EventManager::~EventManager() {};

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                    INPUT EVENTS
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/
/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** addEventListener - add class to listenerList for specified eventType
@param classPtr - reference to class listening to evenType
@param eventType - event to listen for
@return bool - true if added successfully 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool EventManager::addEventListener(InputEventListener& classPtr, sf::Event::EventType eventType) {

	/// Insert EventListener (pointer to class object) into ListenerMap at eventList[eventType]
	/// Address to classPtr is used as key cast as integer
	/// Data is address to classPtr for dispatchEvent
	/// Attempt to get newly inserted value (.second) if returns nothing insert failed
	if (!(*inputEventList[eventType]).insert(std::make_pair(reinterpret_cast<int>(&classPtr), &classPtr)).second) {
		return false;
	}
	return true;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** removeEventListener
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void EventManager::removeEventListener(InputEventListener& classPtr, sf::Event::EventType eventType) {

	// create inputListener structure
	InputListener* inputListener = new InputListener;
	inputListener->key = reinterpret_cast<int>(&classPtr);
	inputListener->eventType = eventType;

	// add to end of deletion list
	inputListenersDeletionList.push_back(inputListener);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** getListeners - return a ListenerMap for given eventType
@param eventType - eventType to get listenerMap for
@return ListenerMap - ListenerMap for eventType
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
InputListenerMap* EventManager::getListeners(sf::Event::EventType eventType) {
	return inputEventList[eventType];
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** dispatchInputEvent - dispatch input events all registered listeners
@param sf::Event - event object to send
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void EventManager::dispatchEvent(sf::Event& inputEvent) {

	/// Get ListenerMap at EventList[int]
	InputListenerMap& mapPtr = *inputEventList[inputEvent.Type];

	/// For each Listener in ListenerMap dispatch event => call classPtr->onInputEvent(event)
	for (InputListenerMap::iterator iter = mapPtr.begin(); iter != mapPtr.end(); ++iter) {
		static_cast<InputEventListener*>((iter->second))->onInputEvent(inputEvent);
	}

	// finished current dispatch queue -> run deletions process if any
	if (inputListenersDeletionList.size() > 0)
		runInputDeletionList();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** runInputDeletionList - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void EventManager::runInputDeletionList() {

	// for each event listener in deletion list -> erase
	for (std::vector<InputListener*>::iterator iter = inputListenersDeletionList.begin(); iter != inputListenersDeletionList.end(); ++iter) {
		/// Get ListenerMap address for eventType
		InputListenerMap& mapPtr = *inputEventList[(*iter)->eventType];
		/// Attempt to delete Listener (key) in mapPtr - returns number of elements deleted which is only 0 or 1 for map
		mapPtr.erase((*iter)->key);
	}

	// clear list
	inputListenersDeletionList.clear();
}

/**\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
                      GAME EVENTS
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\**/
/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** addEventListener - add class to listenerList for specified eventType
@param classPtr - reference to class listening to evenType
@param eventType - event to listen for
@return bool - true if added successfully 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
bool EventManager::addEventListener(GameEventListener& classPtr, GameEvent::eventTypes eventType) {

	/// Insert EventListener (pointer to class object) into ListenerMap at eventList[eventType]
	/// Address to classPtr is used as key cast as integer
	/// Data is address to classPtr for dispatchEvent
	/// Attempt to get newly inserted value (.second) if returns nothing insert failed
	if (!(*gameEventList[eventType]).insert(std::make_pair(reinterpret_cast<int>(&classPtr), &classPtr)).second) {
		return false;
	}
	return true;
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** removeEventListener  - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void EventManager::removeEventListener(GameEventListener& classPtr, GameEvent::eventTypes eventType) {

	// create gameListener structure
	GameListener* gameListener = new GameListener;
	gameListener->key = reinterpret_cast<int>(&classPtr);
	gameListener->eventType = eventType;

	// add to end of deletion list
	gameListenersDeletionList.push_back(gameListener);
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** getListeners - return a ListenerMap for given eventType
@param eventType - eventType to get listenerMap for
@return ListenerMap - ListenerMap for eventType
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
GameListenerMap* EventManager::getListeners(GameEvent::eventTypes eventType) {
	return gameEventList[eventType];
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** dispatchEvent - dispatch general events all registered listeners
@param GameEvent - event object to send
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void EventManager::dispatchEvent(GameEvent& gameEvent) {

	/// Get ListenerMap at EventList[int]
	GameListenerMap& mapPtr = *gameEventList[gameEvent.type];

	/// For each Listener in ListenerMap dispatch event => call classPtr->onGameEvent(event)
	for (GameListenerMap::iterator iter = mapPtr.begin(); iter != mapPtr.end(); ++iter) {
		static_cast<GameEventListener*>((iter->second))->onGameEvent(gameEvent);
	}

	// finished current dispatch queue -> run deletions process if any
	if (gameListenersDeletionList.size() > 0)
		runGameDeletionList();
}

/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
/** runGameDeletionList - 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void EventManager::runGameDeletionList() {

	// for each event listener in deletion list -> erase
	for (std::vector<GameListener*>::iterator iter = gameListenersDeletionList.begin(); iter != gameListenersDeletionList.end(); ++iter) {
		/// Get ListenerMap address for eventType
		GameListenerMap& mapPtr = *gameEventList[(*iter)->eventType];
		/// Attempt to delete Listener (key) in mapPtr - returns number of elements deleted which is only 0 or 1 for map
		mapPtr.erase((*iter)->key);
	}

	// clear list
	gameListenersDeletionList.clear();
}