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

int is_e_valid(struct automate *aut)
{
	double sum = 0;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		sum += *((double *)aut->states[i].b->item2);
	}
	if(sum == 1)
	{
		return 1;
	}
	return 0;
}

struct automate* make_automate(char **states, double *proba_init, double nb_states)
{
	struct automate *aut = malloc(sizeof(struct automate));
	aut->nbstates = nb_states;
	aut->states = malloc(nb_states * sizeof(struct state));
	for(int i = 0; i < nb_states; ++i)
	{
		states[i].name = states[i];
		states[i].p_init = proba_init[i];
	}
	return aut;
}

int main()
{
	//Test Pluie Soleil
	
	//SET TRAMSITION PROBA
	char **states = {"Soleil","Pluie"};
	double *proba_init = {0.6,0.4};i
	double v11 = 0.8;
	double v12 = 0.7;
	double v21 = 0.2;
	double v22 = 0.3;
	struct tuple a* = malloc(2*sizeof(struct tuple));
	a[0].item1 = "Soleil";
	a[0].item2 = &v11;
	a[1].item1 = "Pluie";
	a[1].item2 = &v12;
	struct tuple a1* = malloc(2*sizeof(struct tuple));
	a1[0].item1 = "Soleil";
	a1[0].item2 = &v21;
	a1[1].item1 = "Pluie";
	a1[1].item2 = &v22;
	struct automate *aut = make_automate(states, proba_init, 2);
	aut->states[0].a = a;
	aut->states[1].a1 = a1;
	return 0;	
}
