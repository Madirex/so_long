#include "so_long.h"

static int	player_collision_action(mapdata *map_data, int new_pos)
{
	if (map_data->map[new_pos] == 'C')
	{
		map_data->player_coins++;
		ft_putstr_fd("Player collected a coin.\n", 1);
	}
	else if (map_data->map[new_pos] == 'E')
	{
		if (map_data->player_coins < map_data->map_coins)
		{
			ft_putstr_fd("⚠️ Player needs to collect all coins.\n", 1);
			draw_map(map_data);
			return (0);
		}
		ft_putstr_fd("Player exited the map.\n", 1);
		close_window(map_data);
	}
	return (1);
}

static int	walk(mapdata *map_data, int move_offset)
{
	int	player_pos;
	int	new_pos;
	int	dest_col_index;

	player_pos = map_data->player_pos;
	new_pos = player_pos + move_offset;
	dest_col_index = new_pos % map_data->size_w;
	if (dest_col_index < 0 || dest_col_index >= map_data->size_w
		|| map_data->map[new_pos] == '1')
		return (0);
	if (!player_collision_action(map_data, new_pos))
		return (0);
	if (move_offset == -1)
		map_data->player_dir = 0;
	else if (move_offset == 1)
		map_data->player_dir = 1;
	else if (move_offset == -map_data->size_w)
		map_data->player_dir = 2;
	else if (move_offset == map_data->size_w)
		map_data->player_dir = 3;
	map_data->map[player_pos] = '0';
	map_data->map[new_pos] = 'P';
	map_data->player_pos = new_pos;
	return (1);
}

int	key_hook(int keycode, void *param)
{
	mapdata	*map_data;
	int		direction;

	map_data = (mapdata *)param;
	direction = 0;
	if (keycode == 65307)
	{
		ft_putstr_fd("Escape key pressed. Closing window...\n", 2);
		close_window(param);
	}
	else if (keycode == 119 || keycode == 65362)
		direction = -map_data->size_w;
	else if (keycode == 115 || keycode == 65364)
		direction = map_data->size_w;
	else if (keycode == 97 || keycode == 65361)
		direction = -1;
	else if (keycode == 100 || keycode == 65363)
		direction = 1;
	if (direction && walk(map_data, direction))
	{
		draw_map(param);
		map_data->player_moves++;
	}
	return (0);
}
