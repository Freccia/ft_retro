#include "GameMaster.hpp"
#include <ncurses.h>
#include <signal.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

GameMaster::GameMaster(void) :
begin_time(clock())
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

	// t_time
	this->nEntities++;
	if (this->ennemies) {
		this->ennemies = new GameEntity("(", this->winX - 2, this->winY / 2, -1, 0, this->ennemies);
	}
	else {
		this->ennemies = new GameEntity("(", this->winX - 2, this->winY / 2, -1, 0, NULL);
	}
}

/*
void		GameMaster::displayEntities(void) {
	GameEntity		*ptr = this->ennemies;

	while (ptr) {
		mvwprintw(this->win, ptr->getPosY(), ptr->getPosX(), ptr->getShape().c_str());
		ptr = ptr->next;
	}
}
*/

void		GameMaster::displayBanner(void) {
	this->_time = float(clock() - this->begin_time);
	std::string		msg = std::to_string(this->_time);
	mvprintw(WINBOXY - 2, WINBOXX, msg.c_str());
}

void		GameMaster::manageShootsCollisions(GameEntity *entity_to_check) {

	GameEntity * current;

	current = this->shoots;
	while (current)
	{
		if (entity_to_check->checkCollision(current) == true)
		{
			current->collided = true;
			entity_to_check->collided = true;
		}
		current = current->next;
	}
}

void		GameMaster::moveEnnemies(void) {
	GameEntity		*ptr = this->ennemies;

	while (ptr) {
		mvwprintw(this->win, ptr->getPosY(), ptr->getPosX(), " ");
		ptr->updatePosition(this->winX, this->winY);
		manageShootsCollisions(ptr);
		ptr = ptr->next;
	}
}

void		GameMaster::moveShoots(void) {
	GameEntity		*ptr = this->shoots;

	while (ptr) {
		mvwprintw(this->win, ptr->getPosY(), ptr->getPosX(), " ");
		if (ptr->collided == false)
			ptr->updatePosition(this->winX, this->winY);
		// manageShootsCollisions(ptr);
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
		if (this->pl.getPosX() > 1)
			this->pl.setPosition(this->pl.getPosX() - 1, this->pl.getPosY());
	}
	else if (this->ch == KEY_RIGHT) {
		if (this->pl.getPosX() < this->winX - 2)
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
		if (this->shoots != NULL)
			this->shoots = this->pl.shoot(this->shoots);
		else
			this->shoots = this->pl.shoot(NULL);
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
