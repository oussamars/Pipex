SRCS = pipex.c get_next_line/get_next_line.c get_next_line/get_next_line_utils.c ft_split.c libft.c utils.c

SRCS_BONUS = pipex_bonus/pipex_bonus.c get_next_line/get_next_line.c get_next_line/get_next_line_utils.c libft.c \
ft_split.c pipex_bonus/utils_bonus.c pipex_bonus/here_doc_bonus.c pipex_bonus/handle_child_bonus.c

CC = cc

CFLAGS = -Wall -Wextra -Werror

NAME = pipex

NAME_BONUS = bonus

OBJS = $(SRCS:.c=.o)

OBJS_BONUS = $(SRCS_BONUS:.c=.o)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(NAME_BONUS) : $(OBJS_BONUS)
	@touch bonus
	$(CC) $(CFLAGS) $(OBJS_BONUS) -o $(NAME)

clean:
	rm -f $(OBJS) $(OBJS_BONUS)

fclean: clean
	rm -f $(NAME) bonus

re: fclean all

.PHONY: all clean fclean re

.SECONDARY: $(OBJS) $(OBJS_BONUS)