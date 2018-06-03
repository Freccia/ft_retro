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

	int				_winX;
	int				_winY;

	WINDOW			*_win;
	Player			_pl;
	int				_ch;
	int				_nEntities;
	clock_t			_begin_time;
	clock_t			_timeScore;
	clock_t			_lastTime;
	int				_difficultyLevel;
	unsigned int	_superFancyPoints;

	GameEntity		_scenery[STARS_NB];

	void			initScenery(void);
	void			manageCollisionsWith(GameEntity *entity, GameEntity *list);
	static void		resizeHandler(int sig);

public:
	GameMaster(void);
	~GameMaster(void);

	/*		Members functions		*/
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
