
#include "GameEntity.hpp"
#include <ncurses.h>

/*		UNUSED		*/
GameEntity::GameEntity(void):
_shape("*"),
collided(false),
next(NULL)
{
	this->_pos.x = COLS;
	this->_pos.y = LINES/2;
	this->_dir.x = 1;
	this->_dir.y = 1;
}

GameEntity::GameEntity(
	std::string shape,
	int px, int py,
	int dx, int dy,
	GameEntity *next) :
_shape(shape),
collided(false),
next(next)
{
	this->_pos.x = px;
	this->_pos.y = py;
	this->_dir.x = dx;
	this->_dir.y = dy;
}

GameEntity::~GameEntity(void) {
}

/*		UNUSED		*/
GameEntity::GameEntity(GameEntity const &Cc) {
	*this = Cc;
}

/*		UNUSED		*/
GameEntity		&GameEntity::operator = (GameEntity const &Cc) {
	if (&Cc == this)
		return *this;
	this->_shape = Cc._shape;
	this->_pos.x = Cc._pos.x;
	this->_pos.y = Cc._pos.y;
	return *this;
}


std::string			GameEntity::getShape(void) const {
	return this->_shape;
}

int					GameEntity::getShapeSize(void) const {
	return this->_shape.size();
}

int					GameEntity::getPosX(void) const {
	return this->_pos.x;
}

int					GameEntity::getPosY(void) const {
	return this->_pos.y;
}


bool				GameEntity::setShape(std::string shape) {
	this->_shape = shape;
	return true;
}

bool				GameEntity::setPosition(int x, int y) {
	this->_pos.x = x;
	this->_pos.y = y;
	return true;
}


bool				GameEntity::updatePosition(int winX, int winY) {
	if (this->collided == true)
		return true;
	this->_pos.x += this->_dir.x;
	this->_pos.y += this->_dir.y;
	if (this->_pos.x > winX - 2 || this->_pos.x < 2) {
		this->collided = true;
		return false;
	}
	if (this->_pos.y > winY - 1 || this->_pos.y < 1) {
		this->collided = true;
		return false;
	}
	return true;
}

bool				GameEntity::checkCollision(GameEntity *entity) {
	if (this == entity || entity->collided == true || this->collided == true)
		return false;
	int		x = entity->getPosX();
	int		y = entity->getPosY();
	if (this->_pos.x == x && this->_pos.y == y) {
		return true; // collision happened
	}
	return false; // no collision
}
