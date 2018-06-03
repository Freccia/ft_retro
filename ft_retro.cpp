/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_retro.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <>                                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 15:16:28 by lfabbro           #+#    #+#             */
/*   Updated: 2018/06/03 11:23:17 by lfabbro          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>

#include "GameMaster.hpp"
#include "Player.hpp"

#define DELAY 30000

int			main(void)
{
	GameMaster		GM;

	//windowBox = subwin(stdscr, _maxY + 2, _maxX + 2, (LINES / 2) - (_maxY / 2), (COLS / 2) - (_maxX / 2));
	//box(_windowBox, ACS_VLINE, ACS_HLINE);
	

	while(42)
	{
		GM.displayBanner();
		GM.ch = getch();
		GM.movePlayer();
		GM.spawnEntity();
		GM.moveEnnemies();
		GM.moveShoots();
		if (GM.checkPlayerCollision() == true)
			break ;
		GM.destroyEntitiesCollision(&GM.ennemies);
		GM.destroyEntitiesCollision(&GM.shoots);
		GM.displayAllEntities();
	
		wrefresh(GM.win);
		if (GM.ch == 'q' || GM.ch == 'Q')
			break;

		usleep(30000);
	}

	// TODO: GAMEOVER MESSAGE

	return 0;
}
