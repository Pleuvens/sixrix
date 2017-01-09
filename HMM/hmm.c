# include <stdlib.h>
# include <stdio.h>

# include "hmm.h"

int is_p_init_valid(struct automate *aut)
{
	double sum = 0;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		sum += aut->states[i].p_init;
	}
	if(sum == 1)
	{
		return 1;
	}
	return 0;
}

int is_t_valid(struct automate *aut)
{
	double sum = 0;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		sum += *((double *)aut->states[i].a->item2);
	}
	if(sum == 1)
	{
		return 1;
	}
	return 0;
}

int main()
{
	return 0;	
}
