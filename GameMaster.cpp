#include "GameMaster.hpp"
#include "NastyFive.hpp"
#include <ncurses.h>
#include <signal.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

GameMaster::GameMaster(void) :
_ch(0),
_nEntities(0),
_begin_time(clock()),
_timeScore(0),
_lastTime(0),
_difficultyLevel(DIFFICULTY_LEVEL),
_superFancyPoints(0),
ennemies(NULL),
shoots(NULL)
{
	initscr();
	if (LINES < MIN_TERM_Y || COLS < MIN_TERM_X)
	{
		endwin();
		std::cerr << "Error : the window is too small." << std::endl;
		exit(1);
	}
	noecho();
	cbreak();
	// non-blocking manner: it will return ERR if the key input is not ready
	nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE);
	curs_set(FALSE);

	this->_winY = LINES - (LINES/2);
	this->_winX = COLS - (LINES/2);
	this->_win = subwin(stdscr, this->_winY, this->_winX, WINBOXY, WINBOXX);
	box(_win, '|', '-');

	/*
	this->ennemies = NULL;
	this->shoots = NULL;
	this->_nEntities = 0;
	this->_difficultyLevel = DIFFICULTY_LEVEL;
	this->_lastTime = 0;
	*/

	this->initScenery();

	signal(SIGWINCH, &GameMaster::resizeHandler);
}

GameMaster::~GameMaster(void) {
	endwin();

	/* debug */
	/* GameEntity	*p = this->ennemies;
	int i = 0;
	while (p)
	{
		i++;
		p = p->next;
	}

	p = this->shoots;
	int j = 0;
	while (p)
	{
		j++;
		p = p->next;
	}
	std::cout << "CHAR: " << this->_ch <<std::endl;
	std::cout << "ENNEMIES: " << i <<std::endl;
	std::cout << "SHOOTS: " << j <<std::endl;
	std::cout << "LINES: " << LINES <<std::endl;
	std::cout << "COLS: " << COLS <<std::endl;
	std::cout << "_WINX: " << this->_winX <<std::endl;
	std::cout << "_WINY: " << this->_winY <<std::endl; */
}

void		GameMaster::resizeHandler(int sig) {
	if (sig == SIGWINCH) {
		endwin();
		std::cerr << "Error : resize during the game." << std::endl;
		exit(1);
	}
}

void		GameMaster::getKey(void) {
	this->_ch = getch();
}

int			GameMaster::getCharacter(void) {
	return this->_ch;
}

int			GameMaster::getDifficultyLevel(void) {
	return this->_difficultyLevel;
}

void		GameMaster::spawnEntity(void) {
	srand(std::time(0) * std::time(0));
	int		r = rand() % this->_winY;
	this->_nEntities++;
	if (this->ennemies) {
		if (r % 2)
			this->ennemies = new GameEntity("(", this->_winX - 2, r, -1, 0, this->ennemies);
		else
			this->ennemies = new NastyFive("5", this->_winX - 2, r, -1, 0, this->ennemies);
	}
	else {
		this->ennemies = new GameEntity("(", this->_winX - 2, r, -1, 0, NULL);
	}
}

bool		GameMaster::gameOverBanner(void) {
	int				c = 0;
	std::string		msg = "  GAMEOVER  ";
	mvwprintw(this->_win, (this->_winY / 2) - 5, (this->_winX / 2) - 10, msg.c_str());
	msg = "  Replay ? (Y/y)  ";
	mvwprintw(this->_win, (this->_winY / 2) - 4, (this->_winX / 2) - 10, msg.c_str());
	wrefresh(this->_win);

	c = getchar();
	//while (c == KEY_LEFT || c == KEY_RIGHT || c == KEY_UP || c == KEY_DOWN || c == ' ')
	while (c == ' ') {
		c = getchar();
	}

	msg = "                     ";
	mvwprintw(this->_win, (this->_winY / 2) - 5, (this->_winX / 2) - 10, msg.c_str());
	mvwprintw(this->_win, (this->_winY / 2) - 4, (this->_winX / 2) - 10, msg.c_str());
	mvprintw(WINBOXY - 2, WINBOXX, "                                                                ");
	if (c == 'Y' || c == 'y') {
		// reinitialize all
		this->destroyEntities(&this->ennemies);
		this->destroyEntities(&this->shoots);
		mvwprintw(this->_win, this->_pl.getPosY(), this->_pl.getPosX(), " ");
		this->_pl.setPosition(2, 2);
		this->_begin_time = clock();
		this->_timeScore = 0;
		this->_lastTime = clock();
		this->_ch = 0;
		this->_nEntities = 0;
		this->_difficultyLevel = DIFFICULTY_LEVEL;
		this->_superFancyPoints = 0;
		return true;
	}
	return false;
}

void		GameMaster::displayBanner(void) {
	std::string		msg = "    TIMESCORE: ";
	this->_timeScore = float(clock() - this->_begin_time);
	msg.append(std::to_string(this->_timeScore));
	msg.append("    DIFFICULTY: ");
	msg.append(std::to_string(this->_difficultyLevel));
	msg.append("    SUPERFANCYPOINTS: ");
	msg.append(std::to_string(this->_superFancyPoints));
	mvprintw(WINBOXY - 2, WINBOXX, msg.c_str());
	mvprintw(WINBOXY - 3, WINBOXX, "        ");
}

void		GameMaster::refreshWindow(void) {
	if (clock() - this->_lastTime > DIFFICULTY_SPEED) {
		if (this->_difficultyLevel > 10000)
			this->_difficultyLevel -= 50;
		this->_lastTime = clock();
	}
	wrefresh(this->_win);
}

void		GameMaster::manageCollisionsWith(GameEntity *entity, GameEntity *list) {

	GameEntity * current;

	current = list;
	while (current)
	{
		if (entity->checkCollision(current) == true)
		{
			current->collided = true;
			entity->collided = true;
		}
		current = current->next;
	}
}

void		GameMaster::moveEnnemies(void) {
	GameEntity		*ptr = this->ennemies;

	while (ptr) {
		mvwprintw(this->_win, ptr->getPosY(), ptr->getPosX(), " ");
		ptr->updatePosition(this->_winX, this->_winY);
		manageCollisionsWith(ptr, this->shoots);
		ptr = ptr->next;
	}
}

void		GameMaster::moveShoots(void) {
	GameEntity		*ptr = this->shoots;

	while (ptr) {
		mvwprintw(this->_win, ptr->getPosY(), ptr->getPosX(), " ");
		if (ptr->collided == false)
		{
			ptr->updatePosition(this->_winX, this->_winY);
			manageCollisionsWith(ptr, this->ennemies);
		}
		ptr = ptr->next;
	}
}

void		GameMaster::displayScenery(void) {
	/* scenery */
	for (int i = 0; i < STARS_NB; i++) {
		mvwprintw(this->_win, this->_scenery[i].getPosY(), this->_scenery[i].getPosX(), this->_scenery[i].getShape().c_str());
	}
}

void		GameMaster::displayAllEntities(void) {

	GameEntity		*ptr;
	/* shoots */
	ptr = this->shoots;
	while (ptr) {
		mvwprintw(this->_win, ptr->getPosY(), ptr->getPosX(), ptr->getShape().c_str());
		ptr = ptr->next;
	}
	/* ennemies */
	ptr = this->ennemies;
	while (ptr) {
		mvwprintw(this->_win, ptr->getPosY(), ptr->getPosX(), ptr->getShape().c_str());
		ptr = ptr->next;
	}
}

void		GameMaster::movePlayer(void) {
	GameEntity		*tmp = NULL;

	mvwprintw(this->_win, this->_pl.getPosY(), this->_pl.getPosX(), " ");
	if (this->_ch == KEY_LEFT) {
		if (this->_pl.getPosX() > 2)
			this->_pl.setPosition(this->_pl.getPosX() - 1, this->_pl.getPosY());
	}
	else if (this->_ch == KEY_RIGHT) {
		if (this->_pl.getPosX() < this->_winX - 3)
			this->_pl.setPosition(this->_pl.getPosX() + 1, this->_pl.getPosY());
	}
	else if (this->_ch == KEY_UP) {
		if (this->_pl.getPosY() > 1)
			this->_pl.setPosition(this->_pl.getPosX(), this->_pl.getPosY() - 1);
	}
	else if (this->_ch == KEY_DOWN) {
		if (this->_pl.getPosY() < this->_winY - 2)
			this->_pl.setPosition(this->_pl.getPosX(), this->_pl.getPosY() + 1);
	}
	else if (this->_ch == ' ') {
		if (this->_pl.getPosX() < this->_winX - 3) {
			tmp = this->_pl.shoot(this->shoots);
			this->shoots = (tmp != NULL) ? tmp : this->shoots;
		}
	}
	mvwprintw(this->_win, this->_pl.getPosY(), this->_pl.getPosX(), this->_pl.getShape().c_str());
}

bool			GameMaster::checkPlayerCollision(void) {
	GameEntity		*ptr;

	ptr = this->ennemies;
	while (ptr) {
		if (this->_pl.getPosX() == ptr->getPosX() &&
			this->_pl.getPosY() == ptr->getPosY())
			return true;
		ptr = ptr->next;
	}
	if (this->_pl.getPosX() >= this->_winX - 3) {
		this->_superFancyPoints++;
		mvwprintw(this->_win, this->_pl.getPosY(), this->_pl.getPosX(), " ");
		this->_pl.setPosition(2, this->_pl.getPosY());
	}
	return false;
}

void		GameMaster::destroyEntitiesCollision(GameEntity ** start) {
	GameEntity    *current;
	GameEntity    *suppr;

	if (*start != NULL)
	{
		current = *start;
		while (current->next != NULL)
		{
			if (current->next->collided == true)
			{
				suppr = current->next;
				current->next = current->next->next;
				delete suppr;
				this->_nEntities--;
			}
			else
				current = current->next;
		}
		if (*start != NULL)
		{
			if ((*start)->collided == true)
			{
				suppr = *start;
				*start = (*start)->next;
				delete suppr;
				this->_nEntities--;
			}
		}
	}
}

void	GameMaster::initScenery(void) {
	int		r_y;
	int		r_x;

	srand(std::time(0));
	for (int i = 0; i < STARS_NB; i++)
	{
		r_x = rand() % (this->_winX - 1);
		r_y = rand() % (this->_winY - 1);
		if (r_y <= 1)
			r_y++;
		if (r_x <= 1)
			r_x++;
		this->_scenery[i].setPosition(r_x, r_y);
	}
}

void		GameMaster::destroyEntities(GameEntity ** start) {
	GameEntity    *current;
	GameEntity    *suppr;

	if (*start != NULL)
	{
		current = *start;
		while (current != NULL)
		{
			mvwprintw(this->_win, current->getPosY(), current->getPosX(), " ");
			suppr = current;
			current = current->next;
			delete suppr;
		}
		*start = NULL;
	}
}
