/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anmateo- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 18:51:35 by anmateo-          #+#    #+#             */
/*   Updated: 2024/12/03 18:51:36 by anmateo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	init_map_data(t_mapdata *map_data)
{
	map_data->player_pos = -1;
	map_data->player_dir = 1;
	map_data->player_moves = 0;
	map_data->player_coins = 0;
	map_data->map_coins = 0;
	map_data->size_w = 0;
	map_data->size_h = 0;
	map_data->map_size = 0;
}

int	main(int argc, char *argv[])
{
	int			fd;
	int			chars_count;
	t_mapdata	map_data;

	init_map_data(&map_data);
	if (argc != 2)
		exit_error(NULL, "Use: ./so_long <file>");
	fd = open_file(argv[1]);
	assign_map_data(fd, &map_data);
	chars_count = 0;
	fd = open_file(argv[1]);
	while (chars_count < map_data.map_size
		&& read(fd, &map_data.map[chars_count], 1) == 1)
		if (map_data.map[chars_count] != '\n')
			chars_count++;
	if (!is_exit_reachable(&map_data))
		exit_error(&map_data, "The map has no valid path.");
	draw_map(&map_data);
	if (chars_count != map_data.map_size)
		exit_error(&map_data,
			"The map has an invalid number of chars.");
	close(fd);
	mlx_hook(map_data.img.mlx_win, 17, 0, close_window, &map_data);
	mlx_key_hook(map_data.img.mlx_win, key_hook, &map_data);
	mlx_loop(map_data.img.mlx);
}
