/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 04:08:55 by iqattami          #+#    #+#             */
/*   Updated: 2025/06/02 04:11:24 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	move_forward(t_player *p, t_game *g)
{
	float	speed;
	float	new_x;
	float	new_y;

	speed = 3;
	new_x = p->x + cos(p->angle) * speed;
	new_y = p->y + sin(p->angle) * speed;
	if (!touch(new_x, new_y, g))
	{
		p->x = new_x;
		p->y = new_y;
	}
}

static void	move_backward(t_player *p, t_game *g)
{
	float	speed;
	float	new_x;
	float	new_y;

	speed = 3;
	new_x = p->x - cos(p->angle) * speed;
	new_y = p->y - sin(p->angle) * speed;
	if (!touch(new_x, new_y, g))
	{
		p->x = new_x;
		p->y = new_y;
	}
}

static void	strafe_left(t_player *p, t_game *g)
{
	float	speed;
	float	new_x;
	float	new_y;

	speed = 3;
	new_x = p->x + sin(p->angle) * speed;
	new_y = p->y - cos(p->angle) * speed;
	if (!touch(new_x, new_y, g))
	{
		p->x = new_x;
		p->y = new_y;
	}
}

static void	strafe_right(t_player *p, t_game *g)
{
	float	speed;
	float	new_x;
	float	new_y;

	speed = 3;
	new_x = p->x - sin(p->angle) * speed;
	new_y = p->y + cos(p->angle) * speed;
	if (!touch(new_x, new_y, g))
	{
		p->x = new_x;
		p->y = new_y;
	}
}

void	move_player(t_player *player, t_game *game)
{
	update_angle(player);
	if (player->key_up)
		move_forward(player, game);
	if (player->key_down)
		move_backward(player, game);
	if (player->key_left)
		strafe_left(player, game);
	if (player->key_right)
		strafe_right(player, game);
}
