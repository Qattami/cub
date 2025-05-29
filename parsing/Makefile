CC = cc

CFLAGS = -Wall -Wextra -Werror #-fsanitize=address -g

LFLAGS = -L../mlx/mlx -lmlx -lXext -lX11 -lm -lz

INCLUDES = ../mlx/libmlx.a

SRC = $(wildcard *.c)

OBJ = $(SRC:.c=.o)

NAME = my_program

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(INCLUDES) $(LFLAGS)

clean:
	rm -rf $(OBJ)
	
fclean: clean
	rm -rf $(NAME)

re: fclean all
