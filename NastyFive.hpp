
#ifndef NASTYFIVE_HPP
#define NASTYFIVE_HPP

#include "GameEntity.hpp"

class NastyFive: public GameEntity{
private:
	NastyFive(void);
	NastyFive				&operator = (NastyFive const &Cc);
	NastyFive(NastyFive const &Cc);

public:
	NastyFive(std::string shape, int px, int py, int dx, int dy, GameEntity *next);
	~NastyFive(void);

	virtual bool		updatePosition(int winX, int winY);
};

#endif
