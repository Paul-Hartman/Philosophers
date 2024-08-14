CC := cc

CFLAGS := -Wall -Wextra -Werror


SRCS := philo.c init.c end.c utils.c 


OBJS := $(SRCS:.c=.o)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

NAME := philo

$(NAME): $(OBJS) 
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) 
 
all: $(NAME)

clean:
	@rm -f $(OBJS)
	
fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus