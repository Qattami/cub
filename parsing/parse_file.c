#include "parsing.h"

void free_tab(char **t)
{
    int i;
    i = 0;
    if(t != NULL)
    {
        while(t[i])
        {
            free(t[i]);
            i++;
        }
        free(t);
    }
}

int valid_digit(const char *str)
{
    int i = 0;
    if (str[i] == '-') {
        return 0; 
    }
    while (str[i]) {
        if (ft_isdigit(str[i])) { // ila kan return machi 1 , char machi digit
            return 1;
        }
        i++;
    }
    return 0;
}

char **colors_RGB(char *line)
{  
    char **table;
    int i;
    
    table = ft_split(line + 2, ',');
    i = 0;
    while(table[i])
    {
        if(valid_digit(table[i]))
            return(free_tab(table), write(2, "Invalide RGB\n", 13),NULL);
        i++;
    }
    return table;
}

int v_RGB(int i)
{
    if(i < 0 || i > 255)
        return 1;
    return 0;
}

int store_RGB(char *c, char *s, t_parse *d)
{
    char **t;
    char **l;   
    
    d->f_c = s[0];
    d->next->f_c = c[0];
    t = colors_RGB(s);
    l = colors_RGB(c);
    if(!t || !l)
        return 1;
    d->r = ft_atoi(t[0]);
    d->g = ft_atoi(t[1]);
    d->b = ft_atoi(t[2]);
    d->next->r = ft_atoi(l[0]);
    d->next->g = ft_atoi(l[1]);
    d->next->b = ft_atoi(l[2]);
    if(v_RGB(d->r)== 1 || v_RGB(d->g) == 1 || v_RGB(d->b) == 1
        || v_RGB(d->next->r)== 1 || v_RGB(d->next->g) == 1 || v_RGB(d->next->b) == 1)
            return(free_tab(t), free_tab(l), write(2, "Invalide RGB here\n", 13), 1);
    return (free_tab(t), free_tab(l), 0);
}
