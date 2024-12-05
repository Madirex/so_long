#include "so_long_bonus.h"

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
	map_data->enemy.dir = 0;
	map_data->enemy_steps = 0;
}

void	move_enemy(t_mapdata *map_data, int enemy_dir,
					int enemy_pos, int new_pos)
{
	if (map_data->map[new_pos] && (map_data->map[new_pos] == '0'
			|| map_data->map[new_pos] == 'P'))
	{
		if (map_data->map[new_pos] == 'P')
		{
			ft_putstr_fd("🔥 Player was caught by the enemy.\n", 1);
			close_window(map_data);
		}
		map_data->map[enemy_pos] = '0';
		map_data->map[new_pos] = 'M';
		enemy_pos = new_pos;
		draw_map(map_data);
	}
	else
	{
		if (map_data->map[enemy_pos - 1] == '0'
			|| map_data->map[enemy_pos - 1] == 'P')
			enemy_dir = 0;
		else if (map_data->map[enemy_pos + 1] == '0'
			|| map_data->map[enemy_pos + 1] == 'P')
			enemy_dir = 1;
	}
	map_data->enemy.dir = enemy_dir;
}

int	enemy_loop(t_game_data *game_data)
{
	int	enemy_dir;
	int	move_offset;
	int	enemy_pos;
	int	new_pos;

	enemy_pos = 0;
	enemy_dir = game_data->map_data->enemy.dir;
	while (game_data->map_data->map[enemy_pos]
		&& game_data->map_data->map[enemy_pos] != 'M')
		enemy_pos++;
	if (enemy_dir == 0)
		move_offset = -1;
	else if (enemy_dir == 1)
		move_offset = 1;
	new_pos = enemy_pos + move_offset;
	game_data->frame_count++;
	if (game_data->frame_count >= 520)
	{
		move_enemy(game_data->map_data, enemy_dir, enemy_pos, new_pos);
		game_data->map_data->enemy_steps++;
		game_data->frame_count = 0;
	}
	return (0);
}

void	initialize_game(char *file, t_mapdata *map_data, t_game_data *game_data)
{
	int	fd;
	int	chars_count;

	chars_count = 0;
	init_map_data(map_data);
	fd = open_file(file);
	assign_map_data(fd, map_data);
	fd = open_file(file);
	while (chars_count < map_data->map_size
		&& read(fd, &map_data->map[chars_count], 1) == 1)
		if (map_data->map[chars_count] != '\n')
			chars_count++;
	if (chars_count != map_data->map_size)
		exit_error(map_data, "The map has an invalid number of chars.");
	if (!is_exit_reachable(map_data))
		exit_error(map_data, "The map has no valid path.");
	draw_map(map_data);
	mlx_hook(map_data->img.mlx_win, 17, 0, close_window, map_data);
	mlx_key_hook(map_data->img.mlx_win, key_hook, map_data);
	game_data->map_data = map_data;
	game_data->frame_count = 0;
	mlx_loop_hook(map_data->img.mlx, (int (*)(void *))enemy_loop, game_data);
}

int	main(int argc, char *argv[])
{
	t_mapdata	map_data;
	t_game_data	game_data;

	if (argc != 2)
		exit_error(NULL, "Use: ./so_long <file>");
	initialize_game(argv[1], &map_data, &game_data);
	mlx_loop(map_data.img.mlx);
	return (0);
}
