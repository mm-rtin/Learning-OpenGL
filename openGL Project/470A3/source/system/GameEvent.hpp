#ifndef GAME_EVENT_H
#define GAME_EVENT_H

class GameEvent {

public :

	/**~~~~~~~~~~~~~~~~~~~~~~~~~~~
	/** ENUM Events
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    enum eventTypes {
		SHOOT_EVENT,
		BOMB_EVENT,
		COUNT
    };

	/** ShootEvent
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	struct shootEvent {
		unsigned int type;
		unsigned int power;
		unsigned int speed;
	};

	/** BombEvent
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	struct bombEvent {
		unsigned int type;
		unsigned int power;
		unsigned int radius;
	};

	/** event type
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    eventTypes type;

    union {
        shootEvent shoot;
		bombEvent bomb; 
    };
};

#endif