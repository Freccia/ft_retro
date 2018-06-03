
#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP

#include <iostream>

struct s_vector {
	int		x;
	int		y;
};

class GameEntity {
private:
	/*	UNUSED		*/
	GameEntity				&operator = (GameEntity const &Cc);
	GameEntity(GameEntity const &Cc);

protected:
	std::string			_shape;
	s_vector			_pos;
	s_vector			_dir; // TODO: who needs it?
	clock_t				_lastUpdate;

public:
	GameEntity(void);
	GameEntity(std::string shape, int px, int py, int dx, int dy, GameEntity *next);
	virtual ~GameEntity(void);


	/*		Function members		*/
	std::string			getShape(void) const;
	int					getShapeSize(void) const;
	int					getPosX(void) const;
	int					getPosY(void) const;
	
	bool				setShape(std::string shape);
	bool				setPosition(int x, int y);

	virtual bool		updatePosition(int winX, int winY);
	bool				checkCollision(GameEntity *entity);

	/*		Attributes		*/
	bool				collided;
	GameEntity			*next;
};


#endif
