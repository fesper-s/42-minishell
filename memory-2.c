/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory-2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fesper-s <fesper-s@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 08:57:30 by fesper-s          #+#    #+#             */
/*   Updated: 2023/02/14 08:57:59 by fesper-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_lstcontent(t_line **buffer)
{
	if ((*buffer)->insert_char)
		free_charpp((*buffer)->insert_char);
	if ((*buffer)->path)
		free((*buffer)->path);
	if ((*buffer)->insert_op)
		free((*buffer)->insert_op);
	if ((*buffer)->cmd)
		free((*buffer)->cmd);
	if ((*buffer)->infile)
		free((*buffer)->infile);
	if ((*buffer)->outfile)
		free((*buffer)->outfile);
}

void	lst_free(t_line **lst)
{
	t_line	*buffer;
	t_line	*del;

	buffer = *lst;
	while (buffer)
	{
		free_charpp(buffer->cmds);
		free_lstcontent(&buffer);
		buffer = buffer->next;
	}
	buffer = *lst;
	while (buffer)
	{
		del = buffer;
		buffer = buffer->next;
		free(del);
	}
	*lst = NULL;
}
