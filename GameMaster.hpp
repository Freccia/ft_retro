#ifndef GAMEMASTER_HPP
#define GAMEMASTER_HPP

#include <iostream>
#include "ncurses.h"
#include "GameEntity.hpp"
#include "Player.hpp"

class GameMaster {
private:
	GameMaster(GameMaster const &Cc);
	GameMaster		&operator = (GameMaster const &Cc);

	int				winX;
	int				winY;

public:
	GameMaster(void);
	~GameMaster(void);

	/*		Members functions		*/
	static void		resizeHandler(int sig);
	void			movePlayer(void);
	bool			checkPlayerCollision(void);
	void			spawnEntity(void);
	void			displayEntities(void);

	/*		Attributes		*/
	WINDOW			*win;
	Player			pl;
	int				ch;
	GameEntity		*start;

};

#endif
