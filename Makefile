# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/13 14:02:20 by fesper-s          #+#    #+#              #
#    Updated: 2023/01/02 09:01:37 by fesper-s         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

CFLAGS	= -Wall -Wextra -Werror

#CPPFLAGS = -I /opt/homebrew/Cellar/readline/8.2.1/include

#LDFLAGS	= -L /opt/homebrew/Cellar/readline/8.2.1/lib

CPPFLAGS = -I ~/.brew/Cellar/readline/8.2.1/include

LDFLAGS	= -L ~/.brew/Cellar/readline/8.2.1/lib

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
