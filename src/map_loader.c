#include "so_long.h"

static void process_character(char c, int actual_w, mapdata *map_data, int *is_exit, int *last_line_walled)
{
    if (c != '\n')
    {
        if (c != '1')
        {
            *last_line_walled = 0;
            if (actual_w == 0 || map_data->size_h == 0 || actual_w == map_data->size_w - 1)
                print_error_and_exit(map_data, "The file has invalid characters in the map limits.");
        }
        if (c == 'P')
        {
            if (map_data->player_pos != -1)
                print_error_and_exit(map_data, "The file has more than one player.");
            map_data->player_pos = map_data->size_w * map_data->size_h + actual_w;
        }
        else if (c == 'E')
        {
            if (*is_exit)
                print_error_and_exit(map_data, "The file has more than one exit.");
            *is_exit = 1;
        }
        else if (c == 'C')
            map_data->map_coins++;
        if (is_valid_map_char(c) == 0)
            print_error_and_exit(map_data, "The file has invalid characters.");
    }
}

static void process_new_line(int actual_w, mapdata *map_data, int *last_line_walled)
{
    *last_line_walled = 1;
    if (actual_w == 0)
        print_error_and_exit(map_data, "The file has an invalid number of columns.");
    map_data->size_h = map_data->size_h + 1;
    if (map_data->size_w == 0)
        map_data->size_w = actual_w;
    else if (map_data->size_w != actual_w)
        print_error_and_exit(map_data, "The file has an invalid number of columns.");
}

static void finalize_map(int actual_w, mapdata *map_data, int last_line_walled, int is_exit)
{
    if (is_exit == 0)
        print_error_and_exit(map_data, "The file has no exit.");
    if (map_data->player_pos == -1)
        print_error_and_exit(map_data, "The file has no player.");
    if (actual_w > 0)
    {
        map_data->size_h++;
        if (map_data->size_w == 0)
            map_data->size_w = actual_w;
        else if (map_data->size_w != actual_w)
            print_error_and_exit(map_data, "The file has an invalid number of columns.");
    }
    if (!last_line_walled)
        print_error_and_exit(map_data, "The file has an invalid last line. It must have at least one walkable cell.");
    if (map_data->size_w == 0 || map_data->size_h == 0)
        print_error_and_exit(map_data, "The file has an invalid number of columns or rows.");
}

static void allocate_map(mapdata *map_data)
{
    map_data->map_size = map_data->size_w * map_data->size_h;
    map_data->map = (char *)malloc(map_data->map_size * sizeof(char));
    map_data->img.mlx = mlx_init();
    map_data->img.mlx_win = mlx_new_window(map_data->img.mlx, map_data->size_w * 64, map_data->size_h * 64, "Lite Retro War Game - C Version");
    if (!map_data->map)
        print_error_and_exit(map_data, "Memory allocation error.");
}

void assign_map_data(int fd, mapdata *map_data)
{
    char c;
    int actual_w;
    int is_exit;
    int last_line_walled;

    actual_w = 0;
    is_exit = 0;
    last_line_walled = 1;
    while (read(fd, &c, 1) == 1)
    {
        if (c != '\n')
        {
            process_character(c, actual_w, map_data, &is_exit, &last_line_walled);
            actual_w++;
        }
        else
        {
            process_new_line(actual_w, map_data, &last_line_walled);
            actual_w = 0;
        }
    }
    finalize_map(actual_w, map_data, last_line_walled, is_exit);
    allocate_map(map_data);
}
