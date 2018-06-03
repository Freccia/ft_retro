
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>

#include "GameEntity.hpp"

class Player: public GameEntity {
private:
	Player(Player const &Cc);
	Player				&operator = (Player const &Cc);

	clock_t				_lastShot;

public:
	Player(void);
	virtual ~Player(void);

	GameEntity			*shoot(GameEntity *next);

};

#endif
