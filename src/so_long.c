#include "so_long.h"

static int print_map(mapdata *map_data)
{
    int i;
    int current_w;

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



void draw_stats(mapdata *map_data)
{
    char *move_str;
    char *coin_str;
    char *move_display;
    char *coin_display;

    move_str = ft_itoa(map_data->player_moves);
    coin_str = ft_itoa(map_data->player_coins);
    move_display = ft_strjoin("Moves: ", move_str);
    coin_display = ft_strjoin("Coins: ", coin_str);

    mlx_string_put(map_data->img.mlx, map_data->img.mlx_win, 10, 10, 0xFFFFFF, move_display);
    mlx_string_put(map_data->img.mlx, map_data->img.mlx_win, 10, 30, 0xFFFFFF, coin_display);
    free(move_str);
    free(coin_str);
    free(move_display);
    free(coin_display);
}

char *get_player_sprite(int player_dir)
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

int verify_image_load(mapdata *map_data, t_images *images)
{
    if (!images->wall_img || !images->empty_img || !images->player_img || !images->coin_img || !images->exit_img)
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

void draw_images(mapdata *map_data, t_images *images, int img_size)
{
    char cell;
    int x;
    int y;

    y = 0;
    while (y < map_data->size_h)
    {
        x = 0;
        while (x < map_data->size_w)
        {
            cell = map_data->map[y * map_data->size_w + x];
            if (cell == '1')
                mlx_put_image_to_window(map_data->img.mlx, map_data->img.mlx_win, images->wall_img, x * img_size, y * img_size);
            else if (cell == '0')
                mlx_put_image_to_window(map_data->img.mlx, map_data->img.mlx_win, images->empty_img, x * img_size, y * img_size);
            else if (cell == 'P')
                mlx_put_image_to_window(map_data->img.mlx, map_data->img.mlx_win, images->player_img, x * img_size, y * img_size);
            else if (cell == 'C')
                mlx_put_image_to_window(map_data->img.mlx, map_data->img.mlx_win, images->coin_img, x * img_size, y * img_size);
            else if (cell == 'E')
                mlx_put_image_to_window(map_data->img.mlx, map_data->img.mlx_win, images->exit_img, x * img_size, y * img_size);
            x++;
        }
        y++;
    }
}

void destroy_all_images(mapdata *map_data, t_images *images)
{
    mlx_destroy_image(map_data->img.mlx, images->wall_img);
    mlx_destroy_image(map_data->img.mlx, images->empty_img);
    mlx_destroy_image(map_data->img.mlx, images->player_img);
    mlx_destroy_image(map_data->img.mlx, images->coin_img);
    mlx_destroy_image(map_data->img.mlx, images->exit_img);
}

void draw_map(mapdata *map_data)
{
    t_images images;
    void *buffer_img;
    int img_size;
    char *player_spr;

    img_size = 64;
    print_map(map_data);
    buffer_img = mlx_new_image(map_data->img.mlx, 1, 1);
    player_spr = get_player_sprite(map_data->player_dir);
    images.wall_img = mlx_xpm_file_to_image(map_data->img.mlx, "textures/wall.xpm", &img_size, &img_size);
    images.empty_img = mlx_xpm_file_to_image(map_data->img.mlx, "textures/air.xpm", &img_size, &img_size);
    images.player_img = mlx_xpm_file_to_image(map_data->img.mlx, player_spr, &img_size, &img_size);
    images.coin_img = mlx_xpm_file_to_image(map_data->img.mlx, "textures/coin.xpm", &img_size, &img_size);
    images.exit_img = mlx_xpm_file_to_image(map_data->img.mlx, "textures/goal.xpm", &img_size, &img_size);
    if (!verify_image_load(map_data, &images))
    {
        mlx_destroy_image(map_data->img.mlx, buffer_img);
        print_error_and_exit(map_data, "Error loading textures.");
    }
    draw_images(map_data, &images, img_size);
    draw_stats(map_data);
    mlx_put_image_to_window(map_data->img.mlx, map_data->img.mlx_win, buffer_img, 0, 0);
    destroy_all_images(map_data, &images);
    mlx_destroy_image(map_data->img.mlx, buffer_img);
}



void init_map_data(mapdata *map_data)
{
    map_data->player_pos = -1;
    map_data->player_dir = 1;
    map_data->player_moves = 0;
    map_data->player_coins = 0;
    map_data->map_coins = 0;
    map_data->size_w = 0;
    map_data->size_h = 0;
    map_data->map_size = 0;
}

int main(int argc, char *argv[])
{
    int fd;
    int chars_count;
    mapdata map_data;

    init_map_data(&map_data);
    if (argc != 2)
        print_error_and_exit(NULL, "Use: ./so_long <file>");
    fd = open_file(argv[1]);
    assign_map_data(fd, &map_data);
    chars_count = 0;
    fd = open_file(argv[1]);
    while (chars_count < map_data.map_size && read(fd, &map_data.map[chars_count], 1) == 1)
    {
        if (map_data.map[chars_count] != '\n')
            chars_count++;
    }
    if (!is_exit_reachable(&map_data))
        print_error_and_exit(&map_data, "The map has no valid path to the exit.");
    draw_map(&map_data);
    if (chars_count != map_data.map_size)
        print_error_and_exit(&map_data, "The file has an invalid number of characters.");
    close(fd);
	mlx_hook(map_data.img.mlx_win, 17, 0, close_window, &map_data);
	mlx_key_hook(map_data.img.mlx_win, key_hook, &map_data);
	mlx_loop(map_data.img.mlx);
}