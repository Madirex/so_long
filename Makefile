NAME        = so_long
SRCS_DIR    = ./src/
OBJS_DIR    = ./obj/
LIB_DIR     = ./libs/
INCLUDE     = -I./include -I$(LIB_DIR)minilibx-linux -I$(LIB_DIR)libft
MFLAGS      = -L$(LIB_DIR)minilibx-linux -lmlx -lXext -lX11 -lm -lz
CFLAGS      = -Wall -Werror -Wextra

LIBFT_PATH  = $(LIB_DIR)libft/libft.a

SRC_FILES   = utils player_actions pathfinding map_loader image_logic map_draw so_long
OBJS        = $(addprefix $(OBJS_DIR), $(addsuffix .o, $(SRC_FILES)))
OBJS_BONUS  = $(addprefix $(OBJS_DIR), $(addsuffix _bonus.o, $(SRC_FILES)))

REPOS = $(LIB_DIR)libft $(LIB_DIR)minilibx-linux

all: libft minilibx $(NAME)

libft:
	@echo "Cloning libft only if not present"
	@if [ ! -d $(LIB_DIR)libft ]; then \
		git clone https://github.com/Madirex/libft.git $(LIB_DIR)libft; \
	fi

minilibx:
	@echo "Cloning minilibx only if not present"
	@if [ ! -d $(LIB_DIR)minilibx-linux ]; then \
		git clone https://github.com/42Paris/minilibx-linux.git $(LIB_DIR)minilibx-linux; \
	fi

$(NAME): $(OBJS)
	@echo "Compiling $@"
	@make -C $(LIB_DIR)libft
	@make -C $(LIB_DIR)minilibx-linux
	@if [ ! -f $(NAME) ] || [ $(OBJS) -nt $(NAME) ]; then \
		$(CC) $(CFLAGS) $(OBJS) $(LIBFT_PATH) $(MFLAGS) -o $(NAME); \
	fi

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

all: $(REPOS) $(NAME)

bonus: libft minilibx $(OBJS_BONUS)
	@echo "Compiling $@"
	@make -C $(LIB_DIR)libft
	@make -C $(LIB_DIR)minilibx-linux
	@if [ ! -f $(NAME) ] || [ $(OBJS_BONUS) -nt $(NAME) ]; then \
		$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBFT_PATH) $(MFLAGS) -o $(NAME); \
	fi

clean:
	$(RM) -r $(OBJS_DIR)
	@make clean -C $(LIB_DIR)libft
	@make clean -C $(LIB_DIR)minilibx-linux

fclean: clean
	$(RM) $(NAME)
	$(RM) bonus
	@make fclean -C $(LIB_DIR)libft
	@make clean -C $(LIB_DIR)minilibx-linux

re: fclean all

.PHONY: all clean fclean re
