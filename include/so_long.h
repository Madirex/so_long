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