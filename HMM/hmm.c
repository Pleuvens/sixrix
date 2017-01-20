# define _XOPEN_SOURCE 500

# include <stdlib.h>
# include <stdio.h>
# include <string.h>

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
		for(int j = 0; j < aut->nb_states; ++j)
		{
			sum += *((double *)aut->states[i].a[j].item2);
		}
	}
	if(sum == aut->nb_states)
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
		for(int j = 0; j < aut->nb_states; ++j)
		{
			sum += *((double *)aut->states[i].b[j].item2);
		}
	}
	if(sum == aut->nb_states)
	{
		return 1;
	}
	return 0;
}

struct automate* make_automate(char **states, double *proba_init, double *A, int nb_states, 
int nb_obs)
{
	struct automate *aut = malloc(sizeof(struct automate));
	aut->nb_states = nb_states;
	aut->nb_obs = nb_obs;
	aut->states = malloc(nb_states * sizeof(struct state));
	for(int i = 0; i < nb_states; ++i)
	{
		aut->states[i].name = states[i];
		aut->states[i].p_init = proba_init[i];
		aut->states[i].a = malloc(nb_states * sizeof(struct tuple));
		aut->states[i].b = malloc(nb_obs * sizeof(struct tuple));
		int t_index = 0;
		for(int k = 0; k < nb_states; ++k)
		{
			aut->states[i].a[k].item1 = calloc(strlen(states[k]),sizeof(char));
			aut->states[i].a[k].item2 = calloc(1,sizeof(double));
			strncpy(aut->states[i].a[k].item1,states[k],strlen(states[k]));
			*((double*)aut->states[i].a[k].item2) = A[t_index];
			++t_index;
		}
		for(int k = 0; k < nb_obs; ++k)
		{
			aut->states[i].b[k].item1 = calloc(strlen(states[k]),sizeof(char));
			aut->states[i].b[k].item2 = calloc(1,sizeof(double));
			strncpy(aut->states[i].b[k].item1,states[k],strlen(states[k]));
			*((double*)aut->states[i].b[k].item2) = A[t_index];
			++t_index;
		}
	}
	return aut;
}

int main()
{
	//-1 if no link
	int nb_states = 2;
	//Test Pluie Soleil
	int nb_obs = 2;	
	//SET TRAMSITION PROBA
	char *states[] = {"Soleil","Pluie"};
	double proba_init[] = {0.6,0.4};
	
	double A[] = {0.8,0.2,0.7,0.3};
	//double B[] = {};

	struct automate *aut = make_automate(states, proba_init, A, nb_states,nb_obs);
	
	int is_p = is_p_init_valid(aut);
	int is_t = is_t_valid(aut);
	int is_e = is_e_valid(aut);

	printf("is_p: %d\nis_t: %d\nis_e: %d\n",is_p,is_t,is_e);
	
	return 0;	
}
