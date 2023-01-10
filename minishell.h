/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 14:04:49 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/10 14:34:44 by gussoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/ioctl.h>
# include <sys/wait.h>

typedef struct s_line
{
	char			*cmd;
	char			**cmds;
	char			*infile;
	char			*outfile;
	struct s_line	*next;
}					t_line;

int	g_status;

// minishell.c
void	expand_var(char *cmd);
void	cmd_process(t_line **line, char **env);
char	**get_env(char **envp);
void	minishell(char **envp);
int		organize_line(t_line **line);
// check.c
void	check_line(t_line *line);
int		check_space(t_line **line);
void	put_space(t_line **line, int x);
void	check_for_pipes(t_line **line, char **cmds);
//init
void	init_files(t_line **line, char **split);
void	init_cmds(t_line **line, char **split);
void	init_linked_list(t_line **line, char **before_pipe, char **after_pipe);
// error.c
int		print_error(char *str);
void	cmd_error(char *cmd);
void	error_display(char *cmd);
char	*path_error(char *cmd, char **path);
void	dir_error(char *path);
// signal.c
void	signals(void);
void	handle_sigint(int signum);
// utils.c
int		free_str_splited(char **str);
char	*find_path(char *cmd);
char	**ft_trim(char **cmds);
char	**get_cmds(char *cmd);
int		cmds_count(char **split);
// list_utils.c
void	ft_lst_add_back(t_line **lst, t_line *new);
t_line	*ft_lst_new(char **cmds, char *infile, char *outfile);
int		ft_lst_size(t_line *lst);
int		cmds_until_pipe(char **cmds);
t_line	*ft_lst_last(t_line *lst);
// builtins.c
int		handle_pwd_and_env(char **cmds, char **env, char *pwd);
int		handle_builtins(char **cmds, char **env);
// builtins2.c
void	relative_path(char **cmds, char **env, char *pwd, int i);
void	chpwd(char **cmds, char **env, char *pwd, int i);
int		handle_cd(char **cmds, char **env, char *pwd, int i);

#endif
