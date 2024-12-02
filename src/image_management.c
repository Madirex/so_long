#include "so_long.h"

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
