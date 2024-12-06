/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anmateo- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:42:45 by anmateo-          #+#    #+#             */
/*   Updated: 2024/12/06 17:42:46 by anmateo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long_bonus.h"

static void	free_mapdata(void *param)
{
	t_mapdata	*map_data;
	t_data		*img;

	map_data = (t_mapdata *)param;
	img = &map_data->img;
	if (map_data->map)
		free(map_data->map);
	if (img->mlx_win)
		mlx_destroy_window(img->mlx, img->mlx_win);
	if (img->mlx)
	{
		mlx_destroy_display(img->mlx);
		free(img->mlx);
	}
}

void	exit_error(void *param, const char *message)
{
	if (param)
		free_mapdata(param);
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd((char *) message, 2);
	ft_putstr_fd("\n", 2);
	exit(1);
}

int	close_window(void *param)
{
	free_mapdata(param);
	exit(0);
}

int	open_file(const char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		exit_error(NULL, "File not found.");
	return (fd);
}

int	is_valid_map_char(char c)
{
	if (c == '1' || c == '0' || c == 'P' || c == 'E' || c == 'C' || c == 'M')
		return (1);
	return (0);
}
