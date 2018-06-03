/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_retro.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfabbro <>                                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/30 15:16:28 by lfabbro           #+#    #+#             */
/*   Updated: 2018/06/03 15:29:17 by lfabbro          ###   ########.fr       */
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

play:
	while(42)
	{
		GM.displayBanner();
		GM.displayScenery();
		GM.getKey();
		GM.movePlayer();
		GM.spawnEntity();
		GM.moveEnnemies();
		GM.moveShoots();
		if (GM.checkPlayerCollision() == true)
			break ;
		GM.destroyEntitiesCollision(&GM.ennemies);
		GM.destroyEntitiesCollision(&GM.shoots);
		GM.displayAllEntities();

		GM.refreshWindow();
		if (GM.getCharacter() == 'q' || GM.getCharacter() == 'Q')
			break;

		usleep(GM.getDifficultyLevel());
	}

	if (GM.gameOverBanner() == true)
		goto play;

	return 0;
}
