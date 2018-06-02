#include "GameMaster.hpp"
#include <ncurses.h>
#include <signal.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

GameMaster::GameMaster(void)
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
	this->win = subwin(stdscr, this->winY, this->winX, 10, 10);
	box(win, '|', '-');

	this->start = NULL;

	signal(SIGWINCH, &GameMaster::resizeHandler);
}

GameMaster::~GameMaster(void) {
	endwin();
	std::cout << "LINES: " << LINES <<std::endl;
}

void		GameMaster::resizeHandler(int sig) {
	if (sig == SIGWINCH) {
		endwin();
		exit(1);
	}
}

void		GameMaster::spawnEntity(void) {
	if (this->start) {
		GameEntity		*e = new GameEntity("(", LINES/2, COLS/2, -1, 0, this->start);
		this->start = e;
	}
	else
		this->start = new GameEntity("(", LINES/2, COLS/2, -1, 0, NULL);
}

void		GameMaster::displayEntities(void) {
	GameEntity		*ptr = this->start;

	while (ptr) {
		mvwprintw(this->win, ptr->getPosY(), ptr->getPosX(), " ");
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
		if (this->start != NULL)
			this->start = this->pl.shoot(this->start);
		else
			this->start = this->pl.shoot(NULL);
	}
	mvwprintw(this->win, this->pl.getPosY(), this->pl.getPosX(), this->pl.getShape().c_str());
}

bool			GameMaster::checkPlayerCollision(void) {
	GameEntity		*ptr;

	ptr = this->start;
	while (ptr) {
		if (this->pl.getPosX() == ptr->getPosX() &&
			this->pl.getPosY() == ptr->getPosY())
			return true;
		ptr = ptr->next;
	}
	return false;
}
