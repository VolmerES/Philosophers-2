NAME = philo
CC = gcc
SRC = ./init.c ./main.c ./monitor.c ./parsing.c ./philos_routine.c ./safe.c ./utils.c


OBJS = $(SRC:.c=.o)
CFLAGS = -Wall -Wextra -Werror -g3
all : $(NAME)
	@echo "Compilacion PHILO: OK"

$(NAME) : $(OBJS)
	@$(CC) $(OBJS) -o $(NAME)


%.o : %.c
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

bonus: BONUS

.PHONY : clean fclean bonus