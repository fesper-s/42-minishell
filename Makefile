# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/13 14:02:20 by fesper-s          #+#    #+#              #
#    Updated: 2023/01/12 09:39:44 by gussoare         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

CFLAGS	= -g -Wall -Wextra -Werror

CPPFLAGS = -I /opt/homebrew/Cellar/readline/8.2.1/include

LDFLAGS = -L /opt/homebrew/Cellar/readline/8.2.1/lib

CPPFLAGS += -I ~/.brew/opt/readline/include

LDFLAGS += -L ~/.brew/opt/readline/lib

LIBFT	= ./libft/libft.a 

SRCS	= ./main.c ./error.c ./signal.c ./utils.c ./minishell.c ./check.c \
		  ./init_struct.c ./builtins.c ./builtins2.c ./list_utils.c

OBJS	= $(SRCS:.c=.o)

$(NAME):	$(OBJS)
			make -C ./libft
			cc $^ $(CFLAGS) $(LIBFT) -o $@ -lreadline $(CPPFLAGS) $(LDFLAGS)

all:		$(NAME)

clean:
			rm -f $(OBJS)
			make clean -C ./libft

fclean:		clean
			rm -f $(NAME)
			rm -rf $(NAME).dSYM
			make fclean -C ./libft

re:			fclean all

.PHONY:		all clean fclean re
