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
		for(int j = 0; j < aut->nb_k; ++j)
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

struct automate* make_automate(char **states, char **k_name, double *proba_init, 
		double *A,double *B, int nb_states, int nb_obs, int nb_k)
{
	struct automate *aut = malloc(sizeof(struct automate));
	aut->nb_states = nb_states;
	aut->nb_obs = nb_obs;
	aut->nb_k = nb_k;
	aut->states = malloc(nb_states * sizeof(struct state));
	int t_index = 0;
	for(int i = 0; i < nb_states; ++i)
	{
		aut->states[i].name = states[i];
		aut->states[i].p_init = proba_init[i];
		aut->states[i].a = malloc(nb_states * sizeof(struct tuple));
		aut->states[i].b = malloc(nb_k * sizeof(struct tuple));
		t_index = i * nb_states;
		for(int k = 0; k < nb_states; ++k)
		{
			aut->states[i].a[k].item1 = calloc(strlen(states[k]),sizeof(char));
			aut->states[i].a[k].item2 = calloc(1,sizeof(double));
			strncpy(aut->states[i].a[k].item1,states[k],strlen(states[k]));
			*((double*)aut->states[i].a[k].item2) = A[t_index];
			++t_index;
		}
		t_index = i * nb_k;
		for(int k = 0; k < nb_k; ++k)
		{
			aut->states[i].b[k].item1 = calloc(strlen(k_name[k]),sizeof(char));
			aut->states[i].b[k].item2 = calloc(1,sizeof(double));
			strncpy(aut->states[i].b[k].item1,k_name[k],strlen(k_name[k]));
			*((double*)aut->states[i].b[k].item2) = B[t_index];
			++t_index;
		}
	}
	return aut;
}

struct state* search_state(struct automate *aut, char *name)
{
	for(int i = 0; i < aut->nb_states; ++i)
	{
		if(!strcmp(aut->states[i].name,name))
		{
			return &aut->states[i];
		}
	}
	return NULL;
}

double get_obs_prob(struct state *s, char *name, int nb_k)
{
	for(int i = 0; i < nb_k; ++i)
	{
		if(!strcmp(s->b[i].item1,name))
		{
			return *((double*)(s->b[i].item2));
		}
	}
	return 0;
}

double get_trans_prob(struct state *s, char *name, int nb_states)
{
	for(int i = 0; i < nb_states; ++i)
	{
		if(!strcmp(s->a[i].item1,name))
		{
			return *((double*)(s->a[i].item2));
		}
	}
	return 0;
}

double compute(struct automate *aut, char **states, char **obs, int len)
{
	double result = 0;
	struct state *cur_state = search_state(aut,*states);
	result = cur_state->p_init * get_obs_prob(cur_state,*obs,aut->nb_obs);
	//printf("%f %f ", cur_state->p_init, get_obs_prob(cur_state,*obs,aut->nb_obs));
	double t_proba = get_trans_prob(cur_state,states[1],aut->nb_states);
	for(int i = 1; i < len; ++i)
	{
		cur_state = search_state(aut,states[i]);
		result *= t_proba * get_obs_prob(cur_state,obs[i],aut->nb_obs);
		//printf("%f %f ", t_proba, get_obs_prob(cur_state,obs[i],aut->nb_obs));
		if(i < len-1)
		{
			t_proba = get_trans_prob(cur_state,states[i+1],aut->nb_states);
		}
	}
	return result;
}

double forward_recursion(struct automate *aut, char **states, int len, char **obs)
{
	double **alpha = calloc(aut->nb_obs,sizeof(double*));
	struct state *cur = NULL;
	struct state *cur2 = NULL;
	double res = 0;
	*alpha = calloc(len,sizeof(double));
	
	/* INITIALISATION */
	for(int i = 0; i < len; ++i)
	{
		cur = search_state(aut, *states);
		alpha[0][i] = cur->p_init * get_obs_prob(cur,obs[0],aut->nb_obs);
	}
	/* RECURSION */
	for(int k = 1; k < aut->nb_obs; ++k)
	{
		alpha[k] = calloc(len,sizeof(double));
		for(int j = 0; j < len; ++j)
		{
			double val = 0;
			cur2 = search_state(aut,states[j]);
			for(int i = 0; i < len; ++i)
			{
				cur = search_state(aut,states[i]);
				val += alpha[k-1][i] * get_trans_prob(cur,states[j],aut->nb_states);
			}
			alpha[k][j] = val * get_obs_prob(cur2, obs[k], aut->nb_obs);
		}
		free(alpha[k-1]);
	}
	/* END */
	for(int i = 0; i < len; ++i)
	{
		res += alpha[aut->nb_obs-1][i];
	}
	free(alpha[aut->nb_obs-1]);
	free(alpha);
	return res;
}

double backward_recursion(struct automate *aut, char **states, int len, char **obs)
{
	double res = 0;
	double val = 0;
	struct state *cur = NULL;
	double **beta = calloc(sizeof(double*),aut->nb_obs);
	beta[aut->nb_obs-1] = calloc(len,sizeof(double));
	for(int i = 0; i < len; ++i)
	{
		beta[aut->nb_obs-1][i] = 1;
	}
	for(int k = aut->nb_obs-2; k > 0; --k)
	{
		beta[k] = calloc(len,sizeof(double));
		for(int j = 0; j < len; ++j)
		{
			val = 0;
			for(int i = 0; i < len; ++i)
			{
				cur = search_state(aut, states[i]);
				struct state *cur2 = search_state(aut,states[j]);
				val += beta[k+1][i] * get_trans_prob(cur2,states[i],aut->nb_states) * 
				get_obs_prob(cur,obs[k+1],aut->nb_obs);
			}
			beta[k][j] = val;
		}
		free(beta[k+1]);
	}
	for(int i = 0; i < len; ++i)
	{
		cur = search_state(aut, states[i]);
		res += beta[1][i] * cur->p_init * 
		get_obs_prob(cur,obs[0],aut->nb_obs);
	}
	free(beta[1]);
	free(beta);
	return res;
}

double _max(double a,double b)
{
	if(a > b)
	{
		return a;
	} else {
		return b;
	}
}

double viterbi(struct automate *aut, char **states, int len, char **obs,char **path)
{
	struct state *cur = search_state(aut, states[0]);
	struct state *cur2 = NULL;
	double **max = calloc(aut->nb_obs,sizeof(double*));
	int **tab_path = calloc(aut->nb_obs,sizeof(int*));
	*max = calloc(len,sizeof(double));
	*tab_path = calloc(len,sizeof(int));
	double res = 0;
	for(int i = 0; i < len; ++i)
	{
		cur = search_state(aut, states[i]);
		max[0][i] = cur->p_init * get_obs_prob(cur,obs[0],aut->nb_obs);
		tab_path[0][i] = -1;
	}
	for(int k = 1; k < aut->nb_obs; ++k)
	{
		max[k] = calloc(len,sizeof(double));
		tab_path[k] = calloc(len,sizeof(int));
		for(int j = 0; j < len; ++j)
		{
			cur2 = search_state(aut,states[j]);
			double val_max = 0;
			for(int i = 0; i < len; ++i)
			{
				cur = search_state(aut,states[i]);
				double tmp = get_trans_prob(cur,states[j],aut->nb_states) * 
				get_obs_prob(cur2,obs[k],aut->nb_obs) * max[k-1][i];
				val_max = _max(val_max, tmp);
			}
			max[k][j] = val_max;
			/*PATH*/
			for(int i = 0; i < len; ++i)
			{
				cur = search_state(aut,states[i]);
				double tmp = get_trans_prob(cur,states[j],aut->nb_states) * 
				get_obs_prob(cur2,obs[k],aut->nb_obs) * max[k-1][i];
				if(tmp == val_max)
				{
					tab_path[k][j] = i;
					break;
				}
			}
			/*----*/
		}
		//free(max[k-1]);
	}
	printf("\n");	
	for(int k = 0; k < aut->nb_obs; ++k)
	{
		for(int i = 0; i < len; ++i)
		{
			printf("%f ",max[k][i]);
		}
		printf("\n");
	}
	for(int k = 0; k < aut->nb_obs; ++k)
	{
		for(int i = 0; i < len; ++i)
		{
			printf("%d ",tab_path[k][i]);
		}
		printf("\n");
	}
	for(int i = 0; i < len; ++i)
	{
		res = _max(res,max[aut->nb_obs-1][i]);
	}
	/* TEST Backtrack */
	int previous = -1;
	for(int i = 0; i < len; ++i)
	{
		if(max[aut->nb_obs-1][i] == res)
		{
			path[aut->nb_obs-1] = states[i];
			previous = i;
			break;
		}
	}
	for(int k = aut->nb_obs-2; k > -1; --k)
	{
		path[k] = states[tab_path[k+1][previous]];
		previous = tab_path[k+1][previous];
	}

	printf("\n");
	free(max[aut->nb_obs-1]);
	free(max);
	return res;
}

int main()
{
	/* TEST 1
	//-1 if no link
	int nb_states = 2;
	//Test Pluie Soleil
	int nb_obs = 5;	
	int nb_k = 2;
	//SET TRAMSITION PROBA
	char *states[] = {"Soleil","Pluie"};
	double proba_init[] = {0.6,0.4};

	char *obj[] = {"13","21"};

	char *obs[] = {"13","21","13","13","21"};
	char *test_states[] = {"Soleil","Pluie","Pluie","Pluie","Soleil"};


	double A[] = {0.8,0.2,0.7,0.3};
	double B[] = {0.4,0.6,0.9,0.1};

	struct automate *aut = make_automate(states, obj, proba_init, A, B, nb_states,nb_obs, nb_k);
	*/
	/* TEST 2 */
	int nb_states = 2;
	//Test Pluie Soleil
	int nb_obs = 4;	
	int nb_k = 3;
	//SET TRAMSITION PROBA
	char *states[] = {"Hot ","Cold"};
	double proba_init[] = {0.6,0.4};

	char *obj[] = {"S","M","L"};

	char *obs[] = {"S","M","S","L"};
	char *test_states[] = {"Hot ","Hot ","Cold","Cold"};
	//char *test_states2[] = {"Cold","Cold","Cold","Hot"};
	int test_len = 4;

	double A[] = {0.7,0.3,0.4,0.6};
	double B[] = {0.1,0.4,0.5,0.7,0.2,0.1};

	struct automate *aut = make_automate(states, obj, proba_init, A, B, nb_states,nb_obs, nb_k);
	
	int is_p = is_p_init_valid(aut);
	int is_t = is_t_valid(aut);
	int is_e = is_e_valid(aut);

	printf("is_p: %d\nis_t: %d\nis_e: %d\n",is_p,is_t,is_e);

	//double res = compute(aut,test_states2,obs, 2);
	//double res = forward_recursion(aut,test_states,test_len,obs);
	//double res = backward_recursion(aut,test_states,test_len,obs);
	char **path = malloc(test_len*sizeof(char*));
	double res = viterbi(aut,test_states,test_len,obs,path);
	printf("res: %f\n",res);
	for(int i = 0; i < test_len; ++i)
	{
		printf("%s ",path[i]);
	}
	printf("\n");
	return 0;	
}
