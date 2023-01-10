/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 12:14:18 by gussoare          #+#    #+#             */
/*   Updated: 2023/01/09 12:14:18 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_lst_add_back(t_line **lst, t_line *new)
{
	t_line	*temp;

	if (!lst)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	temp = ft_lst_last(*lst);
	temp->next = new;
}

t_line	*ft_lst_last(t_line *lst)
{
	if (!lst)
		return (0);
	while (lst->next != 0)
		lst = lst->next;
	return (lst);
}

t_line	*ft_lst_new(char **cmds, char *infile, char *outfile)
{
	t_line	*new;

	new = malloc(sizeof(t_line));
	if (!new)
		return (0);
	new->cmds = cmds;
	new->infile = infile;
	new->outfile = outfile;
	new->next = 0;
	return (new);
}

int	ft_lst_size(t_line *lst)
{
	int	i;

	i = 0;
	while (lst != 0)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

int	cmds_until_pipe(char **cmds)
{
	int	i;

	i = -1;
	while (cmds[++i])
	{
		if (cmds[i][0] == '|')
			break ;
	}
	return (i);
}