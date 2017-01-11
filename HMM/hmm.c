# define _XOPEN_SOURCE 500

# include <stdlib.h>
# include <stdio.h>

# include "hmm.h"

double get_random(int min, int max)
{
	return (double)((rand() % (max + 1 - min)) + min);
}

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
	aut->nb_states = nb_states;
	aut->states = malloc(nb_states * sizeof(struct state));
	for(int i = 0; i < nb_states; ++i)
	{
		aut->states[i].name = states[i];
		aut->states[i].p_init = proba_init[i];
	}
	return aut;
}

int main()
{
	//Test Pluie Soleil
	
	//SET TRAMSITION PROBA
	char **states = malloc(2 * sizeof(char*)); 
	states[0] = "Soleil";
	states[1] = "Pluie";
	double *proba_init = malloc(2*sizeof(double));
	proba_init[0] = 0.6;
	proba_init[1] = 0.4;
	double v11 = 0.8;
	double v12 = 0.7;
	double v21 = 0.2;
	double v22 = 0.3;
	struct tuple *a = malloc(2*sizeof(struct tuple));
	a[0].item1 = "Soleil";
	a[0].item2 = &v11;
	a[1].item1 = "Pluie";
	a[1].item2 = &v12;
	struct tuple *a1 = malloc(2*sizeof(struct tuple));
	a1[0].item1 = "Soleil";
	a1[0].item2 = &v21;
	a1[1].item1 = "Pluie";
	a1[1].item2 = &v22;
	struct automate *aut = make_automate(states, proba_init, 2);
	aut->states[0].a = a;
	aut->states[1].a = a1;
	
	int is_p = is_p_init_valid(aut);
	int is_t = is_t_valid(aut);
	//int is_e = is_e_valid(aut);

	printf("is_p: %d\nis_t: %d\n",is_p,is_t);
	
	return 0;	
}
