/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gussoare <gussoare@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 14:04:49 by fesper-s          #+#    #+#             */
/*   Updated: 2023/01/17 10:40:01 by gussoare         ###   ########.fr       */
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
	int				infile_id;
	int				outfile_id;
	struct s_line	*next;
}					t_line;

typedef struct s_env
{
	char	**env;
}			t_env;

int	g_status;

// minishell.c
void	expand_var(t_line **line, t_env *env);
void	pipeline(t_line **line, int size);
void	cmd_process(t_line **line, t_env **env);
char	**get_env(char **envp);
void	minishell(char **envp);
//check.c
int		organize_line(t_line **line);
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
void	dir_error(char *path);
void	export_error(char *str);
// signal.c
void	signals(void);
void	handle_sigint(int signum);
// utils.c
int		free_str_splited(char **str);
char	*find_path(char *cmd);
char	**ft_trim(char **cmds);
char	**get_cmds(char *cmd);
void	lst_free(t_line **lst);
int		cmds_count(char **split);
// list_utils.c
void	ft_lst_add_back(t_line **lst, t_line *new);
t_line	*ft_lst_new(char **cmds, char *infile, char *outfile);
int		ft_lst_size(t_line *lst);
int		cmds_til_pipe(char **cmds);
t_line	*ft_lst_last(t_line *lst);
// builtins.c
int		handle_pwd_and_env(char **cmds, t_env *env);
int		handle_exports(char **cmds, t_env **env);
int		handle_builtins(char **cmds, t_env **env);
// builtins2.c
void	relative_path(char *cmd, t_env **env, char *pwd, int j);
void	chpwd(char *cmd, t_env **env, int j);
char	*tilde_home(char *cmd, char *home);
int		handle_cd(char **cmds, t_env **env);

#endif
