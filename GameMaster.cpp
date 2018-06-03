#include "GameMaster.hpp"
#include "NastyFive.hpp"
#include <ncurses.h>
#include <signal.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

GameMaster::GameMaster(void) :
begin_time(clock()),
timeScore(0)
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

	this->winY = LINES - (LINES/2);
	this->winX = COLS - (LINES/2);
	this->win = subwin(stdscr, this->winY, this->winX, WINBOXY, WINBOXX);
	box(win, '|', '-');

	this->ennemies = NULL;
	this->shoots = NULL;
	this->nEntities = 0;
	this->difficultyLevel = DIFFICULTY_LEVEL;
	this->lastTime = clock();

	initScenery();

	signal(SIGWINCH, &GameMaster::resizeHandler);
}

GameMaster::~GameMaster(void) {
	endwin();
	GameEntity	*p = this->ennemies;
	int i = 0;
	while (p)
	{
		i++;
	//	std::cout << i << ": " << p->getPosX() << " " << p->getPosY() <<std::endl;
		p = p->next;
	}

	p = this->shoots;
	int j = 0;
	while (p)
	{
		j++;
	//	std::cout << i << ": " << p->getPosX() << " " << p->getPosY() <<std::endl;
		p = p->next;
	}
	std::cout << "ENNEMIES: " << i <<std::endl;
	std::cout << "SHOOTS: " << j <<std::endl;
	std::cout << "LINES: " << LINES <<std::endl;
	std::cout << "COLS: " << COLS <<std::endl;
	std::cout << "WINX: " << this->winX <<std::endl;
	std::cout << "WINY: " << this->winY <<std::endl;
}

void		GameMaster::resizeHandler(int sig) {
	if (sig == SIGWINCH) {
		endwin();
		std::cerr << "Error : resize during the game." << std::endl;
		exit(1);
	}
}

void		GameMaster::getKey(void) {
	this->ch = getch();
}

int			GameMaster::getCharacter(void) {
	return this->ch;
}

int			GameMaster::getDifficultyLevel(void) {
	return this->difficultyLevel;
}

void		GameMaster::spawnEntity(void) {
	srand(std::time(0) * std::time(0));
	int		r = rand() % this->winY;
	this->nEntities++;
	if (this->ennemies) {
		if (r % 2)
			this->ennemies = new GameEntity("(", this->winX - 2, r, -1, 0, this->ennemies);
		else
			this->ennemies = new NastyFive("5", this->winX - 2, r, -1, 0, this->ennemies);
	}
	else {
		this->ennemies = new GameEntity("(", this->winX - 2, r, -1, 0, NULL);
	}
}

bool		GameMaster::gameOverBanner(void) {
	std::string		msg = "  GAMEOVER  ";
	std::string		msg2 = "  Replay ? (Y/y)  ";
	mvwprintw(this->win, (this->winY / 2) - 5, (this->winX / 2) - 10, msg.c_str());
	mvwprintw(this->win, (this->winY / 2) - 4, (this->winX / 2) - 10, msg2.c_str());
	wrefresh(this->win);
	int c = getchar();
	msg = "                     ";
	mvwprintw(this->win, (this->winY / 2) - 5, (this->winX / 2) - 10, msg.c_str());
	mvwprintw(this->win, (this->winY / 2) - 4, (this->winX / 2) - 10, msg.c_str());
	mvprintw(WINBOXY - 2, WINBOXX, "                                                                ");
	if (c == 'Y' || c == 'y') {
		// reinitialize all
		this->destroyEntities(&this->ennemies);
		this->destroyEntities(&this->shoots);
		mvwprintw(this->win, this->pl.getPosY(), this->pl.getPosX(), " ");
		this->pl.setPosition(2, 2);
		this->ch = 0;
		this->nEntities = 0;
		this->begin_time = clock();
		this->timeScore = 0;
		this->lastTime = clock();
		this->difficultyLevel = DIFFICULTY_LEVEL;
		return true;
	}
	return false;
}

void		GameMaster::displayBanner(void) {
	std::string		msg = "    TIMESCORE: ";
	this->timeScore = float(clock() - this->begin_time);
	msg.append(std::to_string(this->timeScore));
	msg.append("    DIFFICULTY: ");
	msg.append(std::to_string(this->difficultyLevel));
	mvprintw(WINBOXY - 2, WINBOXX, msg.c_str());
	mvprintw(WINBOXY - 3, WINBOXX, "        ");
}

void		GameMaster::refreshWindow(void) {
	if (clock() - this->lastTime > DIFFICULTY_SPEED) {
		if (this->difficultyLevel > 10000)
			this->difficultyLevel -= 50;
		this->lastTime = clock();
	}
	wrefresh(this->win);
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
		mvwprintw(this->win, ptr->getPosY(), ptr->getPosX(), " ");
		ptr->updatePosition(this->winX, this->winY);
		manageCollisionsWith(ptr, this->shoots);
		ptr = ptr->next;
	}
}

void		GameMaster::moveShoots(void) {
	GameEntity		*ptr = this->shoots;

	while (ptr) {
		mvwprintw(this->win, ptr->getPosY(), ptr->getPosX(), " ");
		if (ptr->collided == false)
		{
			ptr->updatePosition(this->winX, this->winY);
			manageCollisionsWith(ptr, this->ennemies);
		}
		ptr = ptr->next;
	}
}

void		GameMaster::displayScenery(void) {
	/* scenery */
	for (int i = 0; i < STARS_NB; i++) {
		mvwprintw(this->win, this->_scenery[i].getPosY(), this->_scenery[i].getPosX(), this->_scenery[i].getShape().c_str());
	}
}

void		GameMaster::displayAllEntities(void) {

	GameEntity		*ptr;
	/* shoots */
	ptr = this->shoots;
	while (ptr) {
		mvwprintw(this->win, ptr->getPosY(), ptr->getPosX(), ptr->getShape().c_str());
		ptr = ptr->next;
	}
	/* ennemies */
	ptr = this->ennemies;
	while (ptr) {
		mvwprintw(this->win, ptr->getPosY(), ptr->getPosX(), ptr->getShape().c_str());
		ptr = ptr->next;
	}
}

void		GameMaster::movePlayer(void) {
	GameEntity		*tmp = NULL;

	mvwprintw(this->win, this->pl.getPosY(), this->pl.getPosX(), " ");
	if (this->ch == KEY_LEFT) {
		if (this->pl.getPosX() > 2)
			this->pl.setPosition(this->pl.getPosX() - 1, this->pl.getPosY());
	}
	else if (this->ch == KEY_RIGHT) {
		if (this->pl.getPosX() < this->winX - 3)
			this->pl.setPosition(this->pl.getPosX() + 1, this->pl.getPosY());
	}
	else if (this->ch == KEY_UP) {
		if (this->pl.getPosY() > 1)
			this->pl.setPosition(this->pl.getPosX(), this->pl.getPosY() - 1);
	}
	else if (this->ch == KEY_DOWN) {
		if (this->pl.getPosY() < this->winY - 2)
			this->pl.setPosition(this->pl.getPosX(), this->pl.getPosY() + 1);
	}
	else if (this->ch == ' ') {
		if (this->pl.getPosX() < this->winX - 3) {
			tmp = this->pl.shoot(this->shoots);
			this->shoots = (tmp != NULL) ? tmp : this->shoots;
		}
	}
	mvwprintw(this->win, this->pl.getPosY(), this->pl.getPosX(), this->pl.getShape().c_str());
}

bool			GameMaster::checkPlayerCollision(void) {
	GameEntity		*ptr;

	ptr = this->ennemies;
	while (ptr) {
		if (this->pl.getPosX() == ptr->getPosX() &&
			this->pl.getPosY() == ptr->getPosY())
			return true;
		ptr = ptr->next;
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
				this->nEntities--;
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
				this->nEntities--;
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
		r_x = rand() % (this->winX - 1);
		r_y = rand() % (this->winY - 1);
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
			mvwprintw(this->win, current->getPosY(), current->getPosX(), " ");
			suppr = current;
			current = current->next;
			delete suppr;
		}
		*start = NULL;
	}
}
