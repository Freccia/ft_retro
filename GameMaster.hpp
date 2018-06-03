#ifndef GAMEMASTER_HPP
#define GAMEMASTER_HPP

#include <iostream>
#include "ncurses.h"
#include "GameEntity.hpp"
#include "Player.hpp"

#define WINBOXX 10
#define WINBOXY 10
#define MIN_TERM_X 50
#define MIN_TERM_Y 30
#define STARS_NB 20
#define DIFFICULTY_SPEED 15000
#define DIFFICULTY_LEVEL 30000

class GameMaster {
private:
	GameMaster(GameMaster const &Cc);
	GameMaster		&operator = (GameMaster const &Cc);

	int				winX;
	int				winY;

	WINDOW			*win;
	Player			pl;
	int				ch;
	int				nEntities;
	clock_t			begin_time;
	clock_t			timeScore;
	clock_t			lastTime;
	int				difficultyLevel;

	GameEntity		_scenery[STARS_NB];

	void			initScenery(void);

public:
	GameMaster(void);
	~GameMaster(void);

	/*		Members functions		*/
	static void		resizeHandler(int sig);
	void			getKey(void);
	int				getCharacter(void);
	int				getDifficultyLevel(void);
	void			movePlayer(void);
	void			moveEnnemies(void);
	void			moveShoots(void);
	bool			checkPlayerCollision(void);
	void			spawnEntity(void);
	void			destroyEntitiesCollision(GameEntity ** start);
	void			destroyEntities(GameEntity ** start);
	void			manageCollisionsWith(GameEntity *entity, GameEntity *list);
	void			displayAllEntities(void);
	void			displayBanner(void);
	void			displayScenery(void);
	bool			gameOverBanner(void);
	void			refreshWindow(void);

	/*		Attributes		*/
	GameEntity		*ennemies;
	GameEntity		*shoots;

};

#endif
