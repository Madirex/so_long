#include "libft.h"
#include "mlx.h"
#include <fcntl.h>

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	void	*mlx;
	void	*mlx_win;
}				t_data;

typedef struct {
    int size_w;
    int size_h;
    int map_size;
    int player_pos;
    int player_dir;
    int player_moves;
    int player_coins;
    int map_coins;
    char *map;
    void *buffer_img;
    t_data img;
} mapdata;

void print_error_and_exit(void *param, const char *message);
int close_window(void *param);
int open_file(const char *path);

int key_hook(int keycode, void *param);

void draw_map(mapdata *map_data);