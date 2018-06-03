#include "GameMaster.hpp"
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
		exit(1);
	}
}

void		GameMaster::spawnEntity(void) {
	srand(std::time(0));
	int		r = rand() % this->winY - 2;
	this->nEntities++;
	if (this->ennemies) {
		this->ennemies = new GameEntity("(", this->winX - 2, r, -1, 0, this->ennemies);
	}
	else {
		this->ennemies = new GameEntity("(", this->winX - 2, r, -1, 0, NULL);
	}
}

void		GameMaster::displayBanner(void) {
	std::string		msg = "    TIMESCORE: ";
	this->timeScore = float(clock() - this->begin_time);
	msg.append(std::to_string(this->timeScore));
	mvprintw(WINBOXY - 2, WINBOXX, msg.c_str());
	mvprintw(WINBOXY - 3, WINBOXX, "        ");
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


void		GameMaster::displayAllEntities(void) {
	GameEntity		*ptr;

	ptr = this->shoots;
	while (ptr) {
		mvwprintw(this->win, ptr->getPosY(), ptr->getPosX(), ptr->getShape().c_str());
		ptr = ptr->next;
	}
	ptr = this->ennemies;
	while (ptr) {
		mvwprintw(this->win, ptr->getPosY(), ptr->getPosX(), ptr->getShape().c_str());
		ptr = ptr->next;
	}
}

void		GameMaster::movePlayer(void) {
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
		if (this->pl.getPosX() < this->winX - 3)
			this->shoots = this->pl.shoot(this->shoots);
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
