#include "so_long_bonus.h"

int	verify_image_load(t_mapdata *map_data, t_images *images)
{
	if (!images->wall_img || !images->empty_img || !images->player_img
		|| !images->coin_img || !images->exit_img || !images->enemy_img)
	{
		if (images->wall_img)
			mlx_destroy_image(map_data->img.mlx, images->wall_img);
		if (images->empty_img)
			mlx_destroy_image(map_data->img.mlx, images->empty_img);
		if (images->player_img)
			mlx_destroy_image(map_data->img.mlx, images->player_img);
		if (images->coin_img)
			mlx_destroy_image(map_data->img.mlx, images->coin_img);
		if (images->exit_img)
			mlx_destroy_image(map_data->img.mlx, images->exit_img);
		if (images->enemy_img)
			mlx_destroy_image(map_data->img.mlx, images->enemy_img);
		return (0);
	}
	return (1);
}

void	draw_img(t_mapdata *map, void *img, int x, int y)
{
	mlx_put_image_to_window(map->img.mlx, map->img.mlx_win, img, x, y);
}

char	*get_cell_img(t_images *images, char cell)
{
	if (cell == '1')
		return (images->wall_img);
	else if (cell == '0')
		return (images->empty_img);
	else if (cell == 'P')
		return (images->player_img);
	else if (cell == 'C')
		return (images->coin_img);
	else if (cell == 'E')
		return (images->exit_img);
	else if (cell == 'M')
		return (images->enemy_img);
	return (images->empty_img);
}

void	draw_images(t_mapdata *map, t_images *images, int img_size)
{
	char	cell;
	int		x;
	int		y;
	char	*img_src;

	y = 0;
	while (y < map->size_h)
	{
		x = 0;
		while (x < map->size_w)
		{
			cell = map->map[y * map->size_w + x];
			img_src = get_cell_img(images, cell);
			draw_img(map, img_src, x * img_size, y * img_size);
			x++;
		}
		y++;
	}
}

void	destroy_all_images(t_mapdata *map_data, t_images *images)
{
	mlx_destroy_image(map_data->img.mlx, images->wall_img);
	mlx_destroy_image(map_data->img.mlx, images->empty_img);
	mlx_destroy_image(map_data->img.mlx, images->player_img);
	mlx_destroy_image(map_data->img.mlx, images->coin_img);
	mlx_destroy_image(map_data->img.mlx, images->exit_img);
	mlx_destroy_image(map_data->img.mlx, images->enemy_img);
}
