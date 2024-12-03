#include "so_long.h"

int	verify_image_load(t_mapdata *map_data, t_images *images)
{
	if (!images->wall_img || !images->empty_img || !images->player_img
		|| !images->coin_img || !images->exit_img)
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
		return (0);
	}
	return (1);
}

void	draw_img(t_mapdata *map, void *img, int x, int y)
{
	mlx_put_image_to_window(map->img.mlx, map->img.mlx_win, img, x, y);
}

void	draw_images(t_mapdata *map, t_images *images, int img_size)
{
	char	cell;
	int		x;
	int		y;

	y = 0;
	while (y < map->size_h)
	{
		x = 0;
		while (x < map->size_w)
		{
			cell = map->map[y * map->size_w + x];
			if (cell == '1')
				draw_img(map, images->wall_img, x * img_size, y * img_size);
			else if (cell == '0')
				draw_img(map, images->empty_img, x * img_size, y * img_size);
			else if (cell == 'P')
				draw_img(map, images->player_img, x * img_size, y * img_size);
			else if (cell == 'C')
				draw_img(map, images->coin_img, x * img_size, y * img_size);
			else if (cell == 'E')
				draw_img(map, images->exit_img, x * img_size, y * img_size);
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
}
