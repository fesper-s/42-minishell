# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/13 14:02:20 by fesper-s          #+#    #+#              #
#    Updated: 2023/01/02 13:29:57 by gussoare         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

CFLAGS	= -Wall -Wextra -Werror

LDFLAGS = -L ~/.brew/opt/readline/lib

CPPFLAGS = -I ~/.brew/opt/readline/include

LIBFT	= ./libft/libft.a 

SRCS	= ./main.c ./error.c ./signal.c ./utils.c

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
			make fclean -C ./libft

re:			fclean all

.PHONY:		all clean fclean re
