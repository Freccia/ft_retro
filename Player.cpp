/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <>                                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 16:13:01 by lfabbro           #+#    #+#             */
/*   Updated: 2018/06/03 16:26:17 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Player.hpp"
#include <ncurses.h>

Player::Player(void):
	GameEntity(">", 2, 2, 0, 0, NULL)
{
	this->_lastShot = clock();
}

Player::~Player(void) {
}

GameEntity			*Player::shoot(GameEntity *next) {
	if (clock() - this->_lastShot < 1000)
		return NULL;
	this->_lastShot = clock();
	return new GameEntity("-", this->_pos.x + 1, this->_pos.y,
			this->_dir.x + 1, this->_dir.y, next);
}
