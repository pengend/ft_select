/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pxia <pxia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/23 15:02:30 by pxia              #+#    #+#             */
/*   Updated: 2015/06/08 23:23:32 by pxia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

void			get_sp(t_select *sl)
{
	t_list	*tmp;
	int		sp;
	int		col;

	col = 0;
	while (col < sl->max_col)
	{
		sp = 0;
		tmp = sl->lc;
		while (tmp != NULL)
		{
			if (tmp->col == col)
				sp = tmp->len > sp ? tmp->len : sp;
			tmp = tmp->next;
		}
		sl->sp[col] = sp;
		col++;
	}
}

void			last_col(t_select sl, int *row, int *col)
{
	t_list *tmp;

	tmp = sl.lc;
	while (tmp->next != NULL)
	{
		if (tmp->col == sl.max_col - 2 && tmp->next->col == 0)
			break ;
		tmp = tmp->next;
	}
	if (tmp->next == NULL)
	{
		*row = tmp->row;
		*col = tmp->col;
		return ;
	}
	*row = tmp->row - 1;
	*col = tmp->col + 1;
}

static char		*get_color(char *name)
{
	struct stat buff;
	int			res;

	if ((res = lstat(name, &buff)) == -1)
		return (WHITE_BLUE);
	if (S_ISDIR(buff.st_mode))
		return (BLUE);
	if (S_ISLNK(buff.st_mode))
		return (MAGENTA);
	if (S_ISFIFO(buff.st_mode))
		return (BRUN);
	if (S_ISSOCK(buff.st_mode))
		return (GREEN);
	if (S_ISBLK(buff.st_mode))
		return (BLUE_CYAN);
	if (S_ISCHR(buff.st_mode))
		return (BLUE_YELLOW);
	if (S_ISREG(buff.st_mode) && (buff.st_mode & S_IXUSR))
		return (RED);
	else if (S_ISREG(buff.st_mode))
		return (WHITE);
	return (WHITE_BLUE);
}

static t_list	**get_tab(int argc, char **argv)
{
	t_list	**lt;
	int		i;

	lt = (t_list**)malloc(sizeof(t_list*) * argc + 1);
	i = 0;
	while (i < argc)
	{
		lt[i] = (t_list*)malloc(sizeof(t_list));
		lt[i]->name = argv[i];
		lt[i]->len = ft_strlen(argv[i]);
		lt[i]->color = get_color(lt[i]->name);
		lt[i]->next = NULL;
		lt[i]->indice = 0;
		i++;
	}
	lt[i] = NULL;
	return (lt);
}

t_select		init_list(int argc, char **argv)
{
	t_select		list;
	struct winsize	ws;

	if ((list.max_col = get_column(argc, &ws)) == 0)
	{
		list.lc = NULL;
		return (list);
	}
	list.max_row = ws.ws_row;
	list.col_len = ws.ws_col;
	list.argc = argc;
	list.lt = get_tab(argc, argv);
	list.lc = get_list(argc, list.lt, list.max_row, list.max_col);
	list.sp = (int*)malloc(sizeof(int) * list.max_col + 1);
	list.fd_tty = open("/dev/tty", O_RDWR);
	get_sp(&list);
	list.enable = check_window_size(list);
	last_col(list, &list.l_row, &list.l_col);
	return (list);
}
