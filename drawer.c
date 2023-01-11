/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 09:18:44 by gussoare          #+#    #+#             */
/*   Updated: 2023/01/11 09:18:44 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int open_drawer(t_line **line, int pipe)
{
	int	list_n;

	list_n = ft_lst_size((*line));
	if ((*line)->infile && pipe == list_n)
	{
		(*line)->infile_id = open((*line)->infile, O_RDONLY);
		if ((*line)->infile_id == -1)
		{
			print_error("Error to open file\n");
			exit(EXIT_FAILURE);
		}
        // if theres a infile to open in the drawer
        return (1);
	}
	if ((*line)->outfile && list_n == 1)
	{
		(*line)->outfile_id = open((*line)->outfile, O_WRONLY | O_CREAT | O_TRUNC,\
			 0777);
		if ((*line)->outfile_id == -1)
		{
			print_error("Error to open file\n");
			exit(EXIT_FAILURE);
		}
        // if theres a outfile to open in the drawer
        return (2);
	}
    // if theres no file to open in the drawer
    return (0);
}
