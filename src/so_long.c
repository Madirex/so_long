#include "libft.h"
#include "mlx.h"
#include "so_long.h"
#include <fcntl.h>

void free_mapdata(void *param)
{
    mapdata *map_data = (mapdata *)param;
    t_data *img = &map_data->img;

    if (map_data->map)
        free(map_data->map);
    if (img->mlx_win)
        mlx_destroy_window(img->mlx, img->mlx_win);
    if (img->mlx)
    {
        mlx_destroy_display(img->mlx);
        free(img->mlx);
    }
}

void print_error_and_exit(void *param, const char *message)
{
    if (param)
        free_mapdata(param);
    ft_putstr_fd("Error\n", 2);
    ft_putstr_fd((char *) message, 2);
    ft_putstr_fd("\n", 2);
    exit(1);
}

int dfs(mapdata *map_data, int pos, int *visited, int target_pos)
{
    int directions[4];
    int i;
    int new_pos;

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
        new_pos = pos + directions[i];
        if (new_pos >= 0 && new_pos < map_data->map_size &&
            map_data->map[new_pos] != '1' && (map_data->map[target_pos] == 'E' || map_data->map[new_pos] != 'E') && visited[new_pos] == 0)
        {
            if (dfs(map_data, new_pos, visited, target_pos))
                return (1);
        }
        i++;
    }
    return (0);
}

int is_exit_reachable(mapdata *map_data)
{
    int *visited;
    visited = (int *)malloc(map_data->map_size * sizeof(int));
    if (!visited)
        print_error_and_exit(map_data, "Memory allocation error.");
    for (int i = 0; i < map_data->map_size; i++)
        visited[i] = 0;
    int player_pos = map_data->player_pos;

    int exit_pos;
    for (int i = 0; i < map_data->map_size; i++)
    {
        if (map_data->map[i] == 'E')
        {
            exit_pos = i;
            break;
        }
    }

    int result = dfs(map_data, player_pos, visited, exit_pos);
    if (result == 0)
    {
        free(visited);
        return (0);
    }

    // Comprobar las monedas
    for (int i = 0; i < map_data->map_size; i++)
    {
        if (map_data->map[i] == 'C')
        {
            for (int j = 0; j < map_data->map_size; j++)
                visited[j] = 0;
            result = dfs(map_data, player_pos, visited, i);
            if (result == 0)
            {
                free(visited);
                return (0);
            }
        }
    }
    free(visited);
    return (1);
}

int print_map(mapdata *map_data)
{
    int i = 0;
    int current_w = 0;
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

void draw_map(mapdata *map_data)
{
    print_map(map_data);

    // Crear una imagen "buffer" para no borrar la ventana
    void *buffer_img = mlx_new_image(map_data->img.mlx, 1, 1);

    // Coordenadas de la celda
    int x, y, cell_x, cell_y;
    int img_width = 64;
    int img_height = 64;

    char *player_spr;

    int player_dir = map_data->player_dir;

    if (player_dir == 0)
        player_spr = "textures/player_left.xpm";
    else if (player_dir == 1)
        player_spr = "textures/player_right.xpm";
    else if (player_dir == 2)
        player_spr = "textures/player_up.xpm";
    else
        player_spr = "textures/player_down.xpm";
    // Cargar las imágenes (esto ocurre cada vez que dibujas el mapa)
    void *wall_img = mlx_xpm_file_to_image(map_data->img.mlx, "textures/wall.xpm", &img_width, &img_height);
    void *empty_img = mlx_xpm_file_to_image(map_data->img.mlx, "textures/air.xpm", &img_width, &img_height);
    void *player_img = mlx_xpm_file_to_image(map_data->img.mlx, player_spr, &img_width, &img_height);
    void *coin_img = mlx_xpm_file_to_image(map_data->img.mlx, "textures/coin.xpm", &img_width, &img_height);
    void *exit_img = mlx_xpm_file_to_image(map_data->img.mlx, "textures/goal.xpm", &img_width, &img_height);

    // Verificar si las imágenes se cargaron correctamente
    if (!wall_img || !empty_img || !player_img || !coin_img || !exit_img)
    {
        // Liberar las imágenes anteriores antes de destruir las nuevas imágenes
        if (wall_img)
            mlx_destroy_image(map_data->img.mlx, wall_img);

        if (empty_img)
            mlx_destroy_image(map_data->img.mlx, empty_img);

        if (player_img)
            mlx_destroy_image(map_data->img.mlx, player_img);

        if (coin_img)
            mlx_destroy_image(map_data->img.mlx, coin_img);

        if (exit_img)
            mlx_destroy_image(map_data->img.mlx, exit_img);

        // Liberar el buffer
        mlx_destroy_image(map_data->img.mlx, buffer_img);

        print_error_and_exit(map_data, "Error loading textures.");
    }

    // Dibujar el mapa en el buffer
    for (y = 0; y < map_data->size_h; y++)
    {
        for (x = 0; x < map_data->size_w; x++)
        {
            char cell = map_data->map[y * map_data->size_w + x];
            cell_x = x * img_width;
            cell_y = y * img_height;

            // Dibujar las imágenes en el buffer
            if (cell == '1') // Pared
                mlx_put_image_to_window(map_data->img.mlx, map_data->img.mlx_win, wall_img, cell_x, cell_y);
            else if (cell == '0') // Espacio vacío
                mlx_put_image_to_window(map_data->img.mlx, map_data->img.mlx_win, empty_img, cell_x, cell_y);
            else if (cell == 'P') // Jugador
                mlx_put_image_to_window(map_data->img.mlx, map_data->img.mlx_win, player_img, cell_x, cell_y);
            else if (cell == 'C') // Moneda
                mlx_put_image_to_window(map_data->img.mlx, map_data->img.mlx_win, coin_img, cell_x, cell_y);
            else if (cell == 'E') // Salida
                mlx_put_image_to_window(map_data->img.mlx, map_data->img.mlx_win, exit_img, cell_x, cell_y);
        }
    }

char *move_str = ft_itoa(map_data->player_moves);
char *coin_str = ft_itoa(map_data->player_coins);

// Creamos las cadenas completas
char *move_display = ft_strjoin("Moves: ", move_str);
char *coin_display = ft_strjoin("Coins: ", coin_str);

// Dibujamos el texto en la ventana
mlx_string_put(map_data->img.mlx, map_data->img.mlx_win, 10, 10, 0xFFFFFF, move_display);
mlx_string_put(map_data->img.mlx, map_data->img.mlx_win, 10, 30, 0xFFFFFF, coin_display);

// Liberamos la memoria de las cadenas temporales
free(move_str);
free(coin_str);
free(move_display);
free(coin_display);


    // Actualizar la ventana con el buffer (sin limpiar la ventana antes)
    mlx_put_image_to_window(map_data->img.mlx, map_data->img.mlx_win, buffer_img, 0, 0);

    // Liberar las imágenes anteriores antes de destruir las nuevas imágenes
    mlx_destroy_image(map_data->img.mlx, wall_img);
    mlx_destroy_image(map_data->img.mlx, empty_img);
    mlx_destroy_image(map_data->img.mlx, player_img);
    mlx_destroy_image(map_data->img.mlx, coin_img);
    mlx_destroy_image(map_data->img.mlx, exit_img);

    // Liberar el buffer
    mlx_destroy_image(map_data->img.mlx, buffer_img);

}

int close_window(void *param)
{
    free_mapdata(param);
    exit(0);
}

int player_collision_action(mapdata *map_data, int new_pos)
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
            ft_putstr_fd("⚠️ Player needs to collect all coins before exiting.\n", 1);
            draw_map(map_data);
            return (0);
        }
        ft_putstr_fd("Player exited the map.\n", 1);
        close_window(map_data);
    }
    return (1);
}

int walk_left(mapdata *map_data)
{
    int player_pos;
    int new_pos;
    int dest_col_index;

    player_pos = map_data->player_pos;
    new_pos = player_pos - 1;
    dest_col_index = new_pos % map_data->size_w;
    if (dest_col_index < 0 || dest_col_index == map_data->size_w - 1 || map_data->map[new_pos] == '1')
        return (0);
    if (!player_collision_action(map_data, new_pos))
        return (0);
    map_data->player_dir = 0;
    map_data->map[player_pos] = '0';
    map_data->map[new_pos] = 'P';
    map_data->player_pos = new_pos;
    return (1);
}

int walk_right(mapdata *map_data)
{
    int player_pos;
    int new_pos;
    int dest_col_index;

    player_pos = map_data->player_pos;
    new_pos = player_pos + 1;
    dest_col_index = new_pos % map_data->size_w;
    if (dest_col_index == 0 || dest_col_index >= map_data->size_w || map_data->map[new_pos] == '1')
        return (0);
    if (!player_collision_action(map_data, new_pos))
        return (0);
    map_data->player_dir = 1;
    map_data->map[player_pos] = '0';
    map_data->map[new_pos] = 'P';
    map_data->player_pos = new_pos;
    return (1);
}

int walk_up(mapdata *map_data)
{
    int player_pos;
    int new_pos;

    player_pos = map_data->player_pos;
    new_pos = player_pos - map_data->size_w;
    if (new_pos < 0 || map_data->map[new_pos] == '1')
        return (0);
    if (!player_collision_action(map_data, new_pos))
        return (0);
    map_data->player_dir = 2;
    map_data->map[player_pos] = '0';
    map_data->map[new_pos] = 'P';
    map_data->player_pos = new_pos;
    return (1);
}

int walk_down(mapdata *map_data)
{
    int player_pos;
    int new_pos;

    player_pos = map_data->player_pos;
    new_pos = player_pos + map_data->size_w;
    if (new_pos >= map_data->map_size || map_data->map[new_pos] == '1')
        return (0);
    if (!player_collision_action(map_data, new_pos))
        return (0);
    map_data->player_dir = 3;
    map_data->map[player_pos] = '0';
    map_data->map[new_pos] = 'P';
    map_data->player_pos = new_pos;
    return (1);
}

int	key_hook(int keycode, void *param __attribute__((unused)))
{
    mapdata *map_data = (mapdata *)param;

	if (keycode == 65307)
	{
		ft_putstr_fd("Escape key pressed. Closing window...\n", 2);
		close_window(param);
	}
    else if (keycode == 119 || keycode == 65362)
    {
        if (walk_up(param))
        {
            draw_map(param);
            map_data->player_moves++;
        }
    }
    else if (keycode == 115 || keycode == 65364)
    {
        if (walk_down(param))
        {
            draw_map(param);
            map_data->player_moves++;
        }
    }
    else if (keycode == 97 || keycode == 65361)
    {
        if (walk_left(param))
        {
            draw_map(param);
            map_data->player_moves++;
        }
    }
    else if (keycode == 100 || keycode == 65363)
    {
        if (walk_right(param))
        {
            draw_map(param);
            map_data->player_moves++;
        }
    }
	return (0);
}

int is_valid_map_char(char c)
{
    if (c == '1' || c == '0' || c == 'P' || c == 'E' || c == 'C')
        return (1);
    return (0);
}

void assign_map_data(int fd, mapdata *map_data)
{
    char c;
    int actual_w;
    int is_exit = 0;
    int last_line_walled = 1;

    actual_w = 0;
    map_data->size_w = 0;
    map_data->size_h = 0;
    
    while (read(fd, &c, 1) == 1)
    {
        if (c != '\n')
        {
            if (c != '1')
            {
                last_line_walled = 0;
                if (actual_w == 0 || map_data->size_h == 0 || actual_w == map_data->size_w - 1)
                    print_error_and_exit(&map_data, "The file has invalid characters in the map limits.");
            }
            if (c == 'P')
            {
                if (map_data->player_pos != -1)
                    print_error_and_exit(&map_data, "The file has more than one player.");
                map_data->player_pos = map_data->size_w * map_data->size_h + actual_w;
            }
            else if (c == 'E')
            {
                if (is_exit)
                    print_error_and_exit(&map_data, "The file has more than one exit.");
                is_exit = 1;
            }
            else if (c == 'C')
                map_data->map_coins++;

            actual_w++;
            if (is_valid_map_char(c) == 0)
                print_error_and_exit(&map_data, "The file has invalid characters.");
        }
        else
        {
            last_line_walled = 1;
            if (actual_w == 0)
                print_error_and_exit(&map_data, "The file has an invalid number of columns.");
            map_data->size_h = map_data->size_h + 1;
            if (map_data->size_w == 0)
                map_data->size_w = actual_w;
            else if (map_data->size_w != actual_w)
                print_error_and_exit(&map_data, "The file has an invalid number of columns.");
            actual_w = 0;
        }
    }

    if (is_exit == 0)
        print_error_and_exit(&map_data, "The file has no exit.");

    if (map_data->player_pos == -1)
        print_error_and_exit(&map_data, "The file has no player.");

    // Procesar la última línea si no termina con '\n'
    if (actual_w > 0)
    {
        map_data->size_h++; // Aseguramos que también se cuenta la última línea
        if (map_data->size_w == 0)
            map_data->size_w = actual_w;
        else if (map_data->size_w != actual_w)
            print_error_and_exit(&map_data, "The file has an invalid number of columns.");
    }

    if (!last_line_walled)
        print_error_and_exit(&map_data, "The file has an invalid last line. It must have at least one walkable cell.");

    if (map_data->size_w == 0 || map_data->size_h == 0)
        print_error_and_exit(&map_data, "The file has an invalid number of columns or rows.");
}

int main(int argc, char *argv[]) {
    mapdata map_data;
    map_data.map_coins = 0;
    map_data.player_moves = 0;
    map_data.player_coins = 0;
    map_data.player_pos = -1;
    map_data.player_dir = 1;
    int fd;

    if (argc != 2)
        print_error_and_exit(NULL, "Use: ./so_long <file>");
    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        print_error_and_exit(NULL, "File not found.");

    assign_map_data(fd, &map_data);
    map_data.map_size = map_data.size_w * map_data.size_h;
    map_data.map = (char *)malloc(map_data.map_size * sizeof(char));

    // Inicialización
	map_data.img.mlx = mlx_init();
    int window_width = map_data.size_w * 64;
    int window_height = map_data.size_h * 64;
	map_data.img.mlx_win = mlx_new_window(map_data.img.mlx, window_width, window_height, "Lite Retro War Game - C Version");

    if (!map_data.map)
        print_error_and_exit(&map_data, "Memory allocation error.");
    int i = 0;
    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        print_error_and_exit(&map_data, "File not found.");
    while (i < map_data.map_size && read(fd, &map_data.map[i], 1) == 1)
    {
        if (map_data.map[i] != '\n')
            i++;
    }

    if (!is_exit_reachable(&map_data))
        print_error_and_exit(&map_data, "The map has no valid path to the exit.");

    draw_map(&map_data);
    if (i != map_data.map_size)
        print_error_and_exit(&map_data, "The file has an invalid number of characters.");
    close(fd);
	mlx_hook(map_data.img.mlx_win, 17, 0, close_window, &map_data);
	mlx_key_hook(map_data.img.mlx_win, key_hook, &map_data);
	mlx_loop(map_data.img.mlx);
}