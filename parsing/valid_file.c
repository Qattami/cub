#include "parsing.h"

int get_f_c(t_parse *d, char *s)
{
    char **tab;
    int i;
    int j;

    i = 0;
    j = 0;
    tab = get_6_lines();
    if (!tab)
        return (write(2,"Failed to read lines\n", 21), 1);
    if(extension(s, tab))
        return 1;
    while(tab[i])
    {
        if(!ft_strncmp(tab[i], "C", 1))
            break;
        i++;
    }
    while(tab[j])
    {
        if(!ft_strncmp(tab[j], "F", 1))
            break;
        j++;
    }    
    if(store_RGB(tab[i], tab[j], d))
        return(free_tab(tab), 1);
    return (free_tab(tab), 0);
}

int extension(char *s1, char **tab)
{
    int i;

    i = 0;
    if(pars_extension(s1, ".cub"))
        return(free_tab(tab), write(1, "invalide extension\n", 19), 1);
    while(tab[i])
    {
        if(!ft_strncmp(tab[i], "SO", 2) || !ft_strncmp(tab[i], "NO", 2)
            || !ft_strncmp(tab[i], "WE", 2) || !ft_strncmp(tab[i], "EA", 2))
                {
                    if(pars_extension(tab[i], ".xpm"))
                    return(free_tab(tab), write(1, "invalide extension\n", 19), 1);
                }
        i++;
    }
    if(valid_trend(tab))
        return(free_tab(tab), write(1, "invalide extension\n", 19), 1);
    return(0);
}