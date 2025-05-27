

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif

typedef struct s_list
{
	char			*content;
	struct s_list	*next;
}	t_list;

char	*get_next_line(int fd);
char	*ft_strchr(char *s, int c);
char	*ft_line(t_list **lst);
size_t	ft_strlen(char *str);
char	*ft_strdup(char *s1);

#endif
