

#include "NastyFive.hpp"

NastyFive::NastyFive(
	std::string shape,
	int px, int py,
	int dx, int dy,
	GameEntity *next) :
	GameEntity(shape, px, py, dx, dy, next)
{}

NastyFive::~NastyFive(void) {
}

bool		NastyFive::updatePosition(int winX, int winY) {
	if (this->collided == true)
		return true;
	srand(time(0));
	int		r = rand();
	if (r % 2)
		this->_dir.y = -1;
	else
		this->_dir.y = 1;
	this->_pos.x += this->_dir.x;
	this->_pos.y += this->_dir.y;
	if (this->_pos.x > winX - 2 || this->_pos.x < 2) {
		this->collided = true;
		return false;
	}
	if (this->_pos.y > winY - 2 || this->_pos.y < 1) {
		this->collided = true;
		return false;
	}
	return true;
}
