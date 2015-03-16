#include "ft_select.h"

int term_set(struct termios *term)
{
  term->c_lflag &= ~(ICANON);
  term->c_lflag &= ~(ECHO);
  term->c_cc[VMIN] = 1;
  term->c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSADRAIN, term))
    return (0);
  return (1);
}

int     voir_touche(t_select *sl)
{
  char     buffer[3];
  t_pos pos;
  char *res;

  pos.x = 0;
  pos.y = 0;
  put_command("cl", 0);
  print_list(*sl);
  while (1)
    {
      bzero(buffer, 3);
      read(0, buffer, 3);
      if (buffer[0] == 27)
	direction_key(sl, buff[2]);
      else if (buffer[0] == 4)
	{
	  printf("Ctlr+d, on quitte !\n");
	  return (0);
	}
      tmp = sl;
    }
  return (0);
}

int exit_select(struct termios *term)
{
  term->c_lflag = (ICANON | ECHO);
  tcsetattr(0, 0, term);
  return (0);
}

void signal_handler(void)
{
  signal(SIGWINCH, sig_winch);
}

int main(int argc, char **argv, char **env)
{
  char *name;
  struct termios term;
  t_select sl;

  signal_handler();
  if ((name = getenv("TERM")) == NULL)
    return (0);
  if (tgetent(NULL, name) == ERR)
    return (0);
  if (tcgetattr(0, &term) == -1)
    return (0);
  if (!term_set(&term))
    return (0);
  sl = init_list(argc - 1, argv + 1);
  if (sl.lc == NULL)
    return (exit_select(&term));
  tmp = &sl;
  /*  while (sl.lc != NULL)
    {
      printf("%s  ", sl.lc->name);
      sl.lc = sl.lc->next;
    }
    printf("\n");*/
  voir_touche(&sl);
  return (exit_select(&term));
}
