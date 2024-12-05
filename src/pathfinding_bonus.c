#include "so_long_bonus.h"

static int	dfs(t_mapdata *map_data, int pos, int *visited, int target_pos)
{
	int	directions[4];
	int	i;
	int	new_p;

	directions[0] = -1;
	directions[1] = 1;
	directions[2] = -map_data->size_w;
	directions[3] = map_data->size_w;
	if (pos == target_pos)
		return (1);
	visited[pos] = 1;
	i = 0;
	while (i < 4)
	{
		new_p = pos + directions[i];
		if (new_p >= 0 && new_p < map_data->map_size
			&& map_data->map[new_p] != '1' && (map_data->map[target_pos] == 'E'
				|| map_data->map[new_p] != 'E') && visited[new_p] == 0)
		{
			if (dfs(map_data, new_p, visited, target_pos))
				return (1);
		}
		i++;
	}
	return (0);
}

static void	init_visited(int *visited, int map_size)
{
	int	i;

	i = 0;
	while (i < map_size)
	{
		visited[i] = 0;
		i++;
	}
}

static int	find_exit(t_mapdata *map_data, int *visited)
{
	int	i;

	i = 0;
	while (i < map_data->map_size)
	{
		if (map_data->map[i] == 'E')
			return (dfs(map_data, map_data->player_pos, visited, i));
		i++;
	}
	return (0);
}

static int	check_collectibles(t_mapdata *map_data, int *visited)
{
	int	i;

	i = 0;
	while (i < map_data->map_size)
	{
		if (map_data->map[i] == 'C')
		{
			init_visited(visited, map_data->map_size);
			if (dfs(map_data, map_data->player_pos, visited, i) == 0)
				return (0);
		}
		i++;
	}
	return (1);
}

int	is_exit_reachable(t_mapdata *map_data)
{
	int	*visited;

	visited = (int *)malloc(map_data->map_size * sizeof(int));
	if (!visited)
		exit_error(map_data, "Memory allocation error.");
	init_visited(visited, map_data->map_size);
	if (find_exit(map_data, visited) == 0)
	{
		free(visited);
		return (0);
	}
	if (check_collectibles(map_data, visited) == 0)
	{
		free(visited);
		return (0);
	}
	free(visited);
	return (1);
}
