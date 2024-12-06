/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anmateo- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:41:45 by anmateo-          #+#    #+#             */
/*   Updated: 2024/12/06 17:41:47 by anmateo-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "mlx.h"
#include <fcntl.h>

typedef struct s_enemy
{
	int	dir;
}	t_enemy;

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	void	*mlx;
	void	*mlx_win;
}	t_data;

typedef struct s_mapdata
{
	int		size_w;
	int		size_h;
	int		map_size;
	int		player_pos;
	int		player_dir;
	int		player_moves;
	int		player_coins;
	int		map_coins;
	char	*map;
	void	*buffer_img;
	int		enemy_steps;
	int		enemy_count;
	t_enemy	enemy;
	t_data	img;
}	t_mapdata;

typedef struct s_images
{
	void	*wall_img;
	void	*empty_img;
	void	*player_img;
	void	*coin_img;
	void	*exit_img;
	void	*enemy_img;
}	t_images;

typedef struct s_status
{
	int	is_exit;
	int	last_line_walled;
}	t_status;

typedef struct s_game_data
{
	t_mapdata	*map_data;
	int			frame_count;
}	t_game_data;

void	exit_error(void *param, const char *message);
int		close_window(void *param);
int		open_file(const char *path);
int		is_valid_map_char(char c);

int		key_hook(int keycode, void *param);

int		is_exit_reachable(t_mapdata *map_data);

void	assign_map_data(int fd, t_mapdata *map_data);

int		verify_image_load(t_mapdata *map_data, t_images *images);
void	draw_images(t_mapdata *map_data, t_images *images, int img_size);
void	destroy_all_images(t_mapdata *map_data, t_images *images);

void	draw_map(t_mapdata *map_data);
