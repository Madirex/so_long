#include "so_long.h"

static int	print_map(t_mapdata *map_data)
{
	int	i;
	int	current_w;

	i = 0;
	current_w = 0;
	ft_putchar_fd('\n', 1);
	ft_putstr_fd("Player moves: ", 1);
	ft_putnbr_fd(map_data->player_moves, 1);
	ft_putchar_fd('\n', 1);
	ft_putstr_fd("Player coins: ", 1);
	ft_putnbr_fd(map_data->player_coins, 1);
	ft_putchar_fd('\n', 1);
	while (i < map_data->map_size)
	{
		if (current_w == map_data->size_w)
		{
			ft_putchar_fd('\n', 1);
			current_w = 0;
		}
		ft_putchar_fd(map_data->map[i], 1);
		i++;
		current_w++;
	}
	ft_putchar_fd('\n', 1);
	return (0);
}

static void	draw_stats(t_mapdata *map_data)
{
	char	*move_str;
	char	*coin_str;
	char	*move_display;
	char	*coin_display;

	move_str = ft_itoa(map_data->player_moves);
	coin_str = ft_itoa(map_data->player_coins);
	move_display = ft_strjoin("Moves: ", move_str);
	coin_display = ft_strjoin("Coins: ", coin_str);
	mlx_string_put(map_data->img.mlx,
		map_data->img.mlx_win, 10, 10, 0xFFFFFF, move_display);
	mlx_string_put(map_data->img.mlx,
		map_data->img.mlx_win, 10, 30, 0xFFFFFF, coin_display);
	free(move_str);
	free(coin_str);
	free(move_display);
	free(coin_display);
}

static char	*get_player_sprite(int player_dir)
{
	if (player_dir == 0)
		return ("textures/player_left.xpm");
	else if (player_dir == 1)
		return ("textures/player_right.xpm");
	else if (player_dir == 2)
		return ("textures/player_up.xpm");
	else
		return ("textures/player_down.xpm");
}

void	assign_images(t_mapdata *map_data,
			t_images *images, char *player_spr, int img_size)
{
	images->wall_img = mlx_xpm_file_to_image(map_data->img.mlx,
			"textures/wall.xpm", &img_size, &img_size);
	images->empty_img = mlx_xpm_file_to_image(map_data->img.mlx,
			"textures/air.xpm", &img_size, &img_size);
	images->player_img = mlx_xpm_file_to_image(map_data->img.mlx,
			player_spr, &img_size, &img_size);
	images->coin_img = mlx_xpm_file_to_image(map_data->img.mlx,
			"textures/coin.xpm", &img_size, &img_size);
	images->exit_img = mlx_xpm_file_to_image(map_data->img.mlx,
			"textures/goal.xpm", &img_size, &img_size);
}

void	draw_map(t_mapdata *map_data)
{
	t_images	images;
	void		*buffer_img;
	int			img_size;
	char		*player_spr;

	img_size = 64;
	print_map(map_data);
	buffer_img = mlx_new_image(map_data->img.mlx, 1, 1);
	player_spr = get_player_sprite(map_data->player_dir);
	assign_images(map_data, &images, player_spr, img_size);
	if (!verify_image_load(map_data, &images))
	{
		mlx_destroy_image(map_data->img.mlx, buffer_img);
		exit_error(map_data, "Error loading textures.");
	}
	draw_images(map_data, &images, img_size);
	draw_stats(map_data);
	mlx_put_image_to_window(map_data->img.mlx,
		map_data->img.mlx_win, buffer_img, 0, 0);
	destroy_all_images(map_data, &images);
	mlx_destroy_image(map_data->img.mlx, buffer_img);
}
