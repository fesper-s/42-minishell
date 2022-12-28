# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/13 14:02:20 by fesper-s          #+#    #+#              #
#    Updated: 2022/12/28 16:50:20 by gussoare         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

CFLAGS	= -Wall -Wextra -Werror

LIBFT	= ./libft/libft.a 

SRCS	= ./main.c ./error.c

OBJS	= $(SRCS:.c=.o)

$(NAME):	$(OBJS)
			make -C ./libft
			cc $^ $(CFLAGS) $(LIBFT) -o $@ -lreadline

all:		$(NAME)

clean:
			rm -f $(OBJS)
			make clean -C ./libft

fclean:		clean
			rm -f $(NAME)
			make fclean -C ./libft

re:			fclean all

.PHONY:		all clean fclean re
