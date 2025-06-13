/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iqattami <iqattami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 22:21:42 by ozouine           #+#    #+#             */
/*   Updated: 2025/06/13 02:15:41 by iqattami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	init_player(t_player *player, char direction, t_map *map)
{
	player->x = map->player_x * BLOCK + BLOCK / 2.0f;
	player->y = map->player_y * BLOCK + BLOCK / 2.0f;
	if (direction == 'N')
		player->angle = 3 * PI / 2;
	else if (direction == 'S')
		player->angle = PI / 2;
	else if (direction == 'E')
		player->angle = 0;
	else if (direction == 'W')
		player->angle = PI;
	else
		player->angle = 0;
	player->key_up = false;
	player->key_down = false;
	player->key_left = false;
	player->key_right = false;
	player->left_rotate = false;
	player->right_rotate = false;
}

int	key_press(int keycode, t_player *player)
{
	if (keycode == W)
		player->key_up = true;
	if (keycode == S)
		player->key_down = true;
	if (keycode == A)
		player->key_left = true;
	if (keycode == D)
		player->key_right = true;
	if (keycode == LEFT)
		player->left_rotate = true;
	if (keycode == RIGHT)
		player->right_rotate = true;
	if (keycode == ESC)
		exit(0);
	return (0);
}

int	key_release(int keycode, t_player *player)
{
	if (keycode == W)
		player->key_up = false;
	if (keycode == S)
		player->key_down = false;
	if (keycode == A)
		player->key_left = false;
	if (keycode == D)
		player->key_right = false;
	if (keycode == LEFT)
		player->left_rotate = false;
	if (keycode == RIGHT)
		player->right_rotate = false;
	return (0);
}

void	move_player(t_player *player, t_game *game)
{
	int		speed;
	float	angle_speed;
	float	cos_angle;
	float	sin_angle;
	float	new_x;
	float	new_y;

	speed = 4;
	angle_speed = 0.045;
	cos_angle = cos(player->angle);
	sin_angle = sin(player->angle);
	if (player->left_rotate)
		player->angle -= angle_speed;
	if (player->right_rotate)
		player->angle += angle_speed;
	if (player->angle > 2 * PI)
		player->angle = 0;
	if (player->angle < 0)
		player->angle = 2 * PI;
	if (player->key_up)
	{
		new_x = player->x + cos_angle * speed;
		new_y = player->y + sin_angle * speed;
		if (!touch(new_x, new_y, game))
		{
			player->x = new_x;
			player->y = new_y;
		}
	}
	if (player->key_down)
	{
		new_x = player->x - cos_angle * speed;
		new_y = player->y - sin_angle * speed;
		if (!touch(new_x, new_y, game))
		{
			player->x = new_x;
			player->y = new_y;
		}
	}
	if (player->key_left)
	{
		new_x = player->x + sin_angle * speed;
		new_y = player->y - cos_angle * speed;
		if (!touch(new_x, new_y, game))
		{
			player->x = new_x;
			player->y = new_y;
		}
	}
	if (player->key_right)
	{
		new_x = player->x - sin_angle * speed;
		new_y = player->y + cos_angle * speed;
		if (!touch(new_x, new_y, game))
		{
			player->x = new_x;
			player->y = new_y;
		}
	}
}
