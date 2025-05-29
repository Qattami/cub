#include "parsing.h"

int get_f_c(t_parse *d)
{
    char **tab;
    int i;
    int j;

    i = 0;
    j = 0;
    tab = get_6_lines();
    if (!tab)
        return (write(2,"Failed to read lines\n", 21), 1);
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
    if(valid_trend(tab))
        return (write(2,"Failed to read lines\n", 21), 1);
    store_RGB(tab[i], tab[j], d);
    return (free_tab(tab), 0);
}
