/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_loader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anmateo- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 18:51:15 by anmateo-          #+#    #+#             */
/*   Updated: 2024/12/03 18:51:16 by anmateo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	process_character(char c, int actual_w,
	t_mapdata *map_data, t_status *status)
{
	if (c != '1')
	{
		status->last_line_walled = 0;
		if (actual_w == 0 || map_data->size_h == 0
			|| actual_w == map_data->size_w - 1)
			exit_error(NULL, "Invalid chars in the map limits.");
	}
	if (c == 'P')
	{
		if (map_data->player_pos != -1)
			exit_error(NULL, "The file has more than one player.");
		map_data->player_pos = map_data->size_w * map_data->size_h + actual_w;
	}
	else if (c == 'E')
	{
		if (status->is_exit)
			exit_error(NULL, "The file has more than one exit.");
		status->is_exit = 1;
	}
	else if (c == 'C')
		map_data->map_coins++;
	if (is_valid_map_char(c) == 0)
		exit_error(NULL, "The file has invalid characters.");
}

static void	process_new_line(int actual_w,
			t_mapdata *map_data, t_status *status)
{
	status->last_line_walled = 1;
	if (actual_w == 0)
		exit_error(NULL, "The file has an invalid number of columns.");
	map_data->size_h = map_data->size_h + 1;
	if (map_data->size_w == 0)
		map_data->size_w = actual_w;
	else if (map_data->size_w != actual_w)
		exit_error(NULL, "The file has an invalid number of columns.");
}

static void	finalize_map(int actual_w, t_mapdata *map_data, t_status *status)
{
	if (status->is_exit == 0)
		exit_error(NULL, "The file has no exit.");
	if (map_data->player_pos == -1)
		exit_error(NULL, "The file has no player.");
	if (actual_w > 0)
	{
		map_data->size_h++;
		if (map_data->size_w == 0)
			map_data->size_w = actual_w;
		else if (map_data->size_w != actual_w)
			exit_error(NULL, "The file has an invalid number of columns.");
	}
	if (!status->last_line_walled)
		exit_error(NULL, "The file has an invalid last line.");
	if (map_data->size_w == 0 || map_data->size_h == 0)
		exit_error(NULL, "The file has an invalid number of cols or rows.");
}

static void	allocate_map(t_mapdata *map_data)
{
	map_data->map_size = map_data->size_w * map_data->size_h;
	map_data->map = (char *)malloc(map_data->map_size * sizeof(char));
	map_data->img.mlx = mlx_init();
	map_data->img.mlx_win = mlx_new_window(map_data->img.mlx,
			map_data->size_w * 64, map_data->size_h * 64,
			"Lite Retro War Game - C Version");
	if (!map_data->map)
		exit_error(map_data, "Memory allocation error.");
}

void	assign_map_data(int fd, t_mapdata *map_data)
{
	char		c;
	int			actual_w;
	t_status	status;

	actual_w = 0;
	status.is_exit = 0;
	status.last_line_walled = 1;
	while (read(fd, &c, 1) == 1)
	{
		if (c != '\n')
		{
			process_character(c, actual_w, map_data, &status);
			actual_w++;
		}
		else
		{
			process_new_line(actual_w, map_data, &status);
			actual_w = 0;
		}
	}
	finalize_map(actual_w, map_data, &status);
	allocate_map(map_data);
}
