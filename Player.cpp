/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <>                                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 16:13:01 by lfabbro           #+#    #+#             */
/*   Updated: 2018/06/02 23:10:27 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Player.hpp"
#include <ncurses.h>

Player::Player(void): GameEntity(">", 1, 1, 0, 0, NULL) {
}

Player::~Player(void) {
}

GameEntity			*Player::shoot(GameEntity *next) {
	return new GameEntity("-", this->_pos.x + 1, this->_pos.y,
			this->_dir.x + 1, this->_dir.y, next);
}
