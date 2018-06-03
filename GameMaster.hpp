#ifndef GAMEMASTER_HPP
#define GAMEMASTER_HPP

#include <iostream>
#include "ncurses.h"
#include "GameEntity.hpp"
#include "Player.hpp"

#define WINBOXX 10
#define WINBOXY 10

class GameMaster {
private:
	GameMaster(GameMaster const &Cc);
	GameMaster		&operator = (GameMaster const &Cc);

	int				winX;
	int				winY;

	int				nEntities;
	clock_t			begin_time;
	clock_t			timeScore;

public:
	GameMaster(void);
	~GameMaster(void);

	/*		Members functions		*/
	static void		resizeHandler(int sig);
	void			movePlayer(void);
	void			moveEnnemies(void);
	void			moveShoots(void);
	bool			checkPlayerCollision(void);
	void			spawnEntity(void);
	void			displayEntities(void);
	void			destroyEntitiesCollision(GameEntity ** start);
	void			manageCollisionsWith(GameEntity *entity, GameEntity *list);
	void			displayAllEntities(void);
	void			displayBanner(void);

	/*		Attributes		*/
	WINDOW			*win;
	Player			pl;
	int				ch;
	GameEntity		*ennemies;
	GameEntity		*shoots;

};

#endif
