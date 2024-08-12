CC := cc

CFLAGS := -Wall -Wextra -Werror


SRCS := philo.c

BONUS_SRCS := 


BONUS_OBJS := $(BONUS_SRCS:.c=.o)

OBJS := $(SRCS:.c=.o)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

NAME := philo

$(NAME): $(OBJS) 
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) 
 
all: $(NAME)

bonus: 

$(LIBFT):
	@make -C ./ft_printf
 
clean:
	@rm -f $(OBJS) $(BONUS_OBJS)
	@make clean -C ./ft_printf
	
fclean: clean
	@make fclean -C ./ft_printf
	@rm -f $(NAME) checker

re: fclean all

.PHONY: all clean fclean re bonus