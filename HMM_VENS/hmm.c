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
	aut->states_n = states;
	aut->obs = k_name;
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

double forward_recursion(struct automate *aut, char **states, int len, char **obs, 
		double **alpha)
{
	struct state *cur = NULL;
	struct state *cur2 = NULL;
	double res = 0;
	*alpha = calloc(aut->nb_states,sizeof(double));

	/* INITIALISATION */
	for(int i = 0; i < aut->nb_states; ++i)
	{
		cur = search_state(aut, *aut->states_n);
		alpha[0][i] = cur->p_init * get_obs_prob(cur,obs[0],aut->nb_obs);
	}
	/* RECURSION */
	for(int k = 1; k < aut->nb_obs; ++k)
	{
		alpha[k] = calloc(aut->nb_states,sizeof(double));
		for(int j = 0; j < aut->nb_states; ++j)
		{
			double val = 0;
			cur2 = search_state(aut,aut->states_n[j]);
			for(int i = 0; i < aut->nb_states; ++i)
			{
				cur = search_state(aut,aut->states_n[i]);
				val += alpha[k-1][i] * get_trans_prob(cur,aut->states_n[j],aut->nb_states);
			}
			alpha[k][j] = val * get_obs_prob(cur2, obs[k], aut->nb_obs);
		}
	}
	/* END */
	for(int i = 0; i < aut->nb_states; ++i)
	{
		res += alpha[aut->nb_obs-1][i];
	}
	return res;
}

double backward_recursion(struct automate *aut, char **states, int len, char **obs,
		double **beta)
{
	double res = 0;
	double val = 0;
	struct state *cur = NULL;
	beta[aut->nb_obs-1] = calloc(aut->nb_states,sizeof(double));
	for(int i = 0; i < aut->nb_states; ++i)
	{
		beta[aut->nb_obs-1][i] = 1;
	}
	for(int k = aut->nb_obs-2; k > 0; --k)
	{
		beta[k] = calloc(aut->nb_states,sizeof(double));
		for(int j = 0; j < aut->nb_states; ++j)
		{
			val = 0;
			for(int i = 0; i < aut->nb_states; ++i)
			{
				cur = search_state(aut, aut->states_n[i]);
				struct state *cur2 = search_state(aut,aut->states_n[j]);
				val += beta[k+1][i] * get_trans_prob(cur2,aut->states_n[i],aut->nb_states) * 
					get_obs_prob(cur,obs[k+1],aut->nb_obs);
			}
			beta[k][j] = val;
		}
	}
	beta[0] = calloc(aut->nb_states,sizeof(double));
	for(int i = 0; i < aut->nb_states; ++i)
	{
		cur = search_state(aut, aut->states_n[i]);
		beta[0][i] = beta[1][i] * cur->p_init * 
			get_obs_prob(cur,obs[0],aut->nb_obs);
		res += beta[0][i];
	}
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
	struct state *cur = search_state(aut, aut->states_n[0]);
	struct state *cur2 = NULL;
	double **max = calloc(aut->nb_obs,sizeof(double*));
	int **tab_path = calloc(aut->nb_obs,sizeof(int*));
	*max = calloc(aut->nb_states,sizeof(double));
	*tab_path = calloc(aut->nb_states,sizeof(int));
	double res = 0;
	for(int i = 0; i < len; ++i)
	{
		cur = search_state(aut, aut->states_n[i]);
		max[0][i] = cur->p_init * get_obs_prob(cur,obs[0],aut->nb_obs);
		tab_path[0][i] = -1;
	}
	for(int k = 1; k < aut->nb_obs; ++k)
	{
		max[k] = calloc(aut->nb_states,sizeof(double));
		tab_path[k] = calloc(aut->nb_states,sizeof(int));
		for(int j = 0; j < aut->nb_states; ++j)
		{
			cur2 = search_state(aut,aut->states_n[j]);
			double val_max = 0;
			for(int i = 0; i < aut->nb_states; ++i)
			{
				cur = search_state(aut,aut->states_n[i]);
				double tmp = get_trans_prob(cur,aut->states_n[j],aut->nb_states) * 
					get_obs_prob(cur2,obs[k],aut->nb_obs) * max[k-1][i];
				val_max = _max(val_max, tmp);
			}
			max[k][j] = val_max;
			/*PATH*/
			for(int i = 0; i < aut->nb_states; ++i)
			{
				cur = search_state(aut,aut->states_n[i]);
				double tmp = get_trans_prob(cur,aut->states_n[j],aut->nb_states) * 
					get_obs_prob(cur2,obs[k],aut->nb_obs) * max[k-1][i];
				if(tmp == val_max)
				{
					tab_path[k][j] = i;
					break;
				}
			}
			/*----*/
		}
		free(max[k-1]);
	}
	/*
	printf("\n");
	
	for(int k = 0; k < aut->nb_obs; ++k)
	{
		for(int i = 0; i < aut->nb_states; ++i)
		{
			printf("%f ",max[k][i]);
		}
		printf("\n");
	}
	for(int k = 0; k < aut->nb_obs; ++k)
	{
		for(int i = 0; i < aut->nb_states; ++i)
		{
			printf("%d ",tab_path[k][i]);
		}
		printf("\n");
	}
	*/
	for(int i = 0; i < aut->nb_states; ++i)
	{
		res = _max(res,max[aut->nb_obs-1][i]);
	}
	/* TEST Backtrack */
	int previous = -1;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		if(max[aut->nb_obs-1][i] == res)
		{
			path[aut->nb_obs-1] = aut->states_n[i];
			previous = i;
			break;
		}
	}
	for(int k = aut->nb_obs-2; k > -1; --k)
	{
		path[k] = states[tab_path[k+1][previous]];
		previous = tab_path[k+1][previous];
	}
	
	for(int k = 0; k < aut->nb_obs; ++k)
	{
		free(tab_path[k]);
	}
	free(tab_path);
	printf("\n");
	free(max[aut->nb_obs-1]);
	free(max);
	return res;
}

double xi(struct automate *aut, char **states, char **obs, int len, double **alpha, double **beta,
		int k, int index_i, int index_j)
{
	struct state *cur_i = NULL;
	struct state *cur_j = NULL;
	/* NUMERATEUR */
	cur_i = search_state(aut,aut->states_n[index_i]);
	cur_j = search_state(aut,aut->states_n[index_j]);
	double numerateur =  alpha[k][index_i]  * get_trans_prob(cur_i,aut->states_n[index_j],
			aut->nb_states) * get_obs_prob(cur_j,obs[k+1],aut->nb_obs) *  beta[k+1][index_j];
	/* DENOMINATEUR */ /* FIXME */

	double denominateur = 0;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		cur_i = search_state(aut,aut->states_n[i]);
		for(int j = 0; j < aut->nb_states; ++j)
		{
			cur_j = search_state(aut,aut->states_n[j]);
			denominateur +=  alpha[k][i] * get_trans_prob(cur_i,aut->states_n[j],aut->nb_states) *
				get_obs_prob(cur_j,obs[k+1],aut->nb_obs) * beta[k+1][j];
		}
	}

	return numerateur/denominateur;
}

double gamma(struct automate *aut, double **alpha, double **beta, int len, int k, int index)
{
	/* NUMERATEUR */
	double numerateur = alpha[k][index] * beta[k][index];

	/* DENOMINATEUR */
	double denominateur = 0;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		denominateur += alpha[k][i] * beta[k][i];
	}
	return numerateur / denominateur;
}

void update_aut(struct automate *aut, double *init, double *a, double *b)
{
	int t_index = 0;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		aut->states[i].p_init = init[i];
		t_index = i * aut->nb_states;
		for(int k = 0; k < aut->nb_states; ++k)
		{
			*((double*)aut->states[i].a[k].item2) = a[t_index];
			++t_index;
		}
		t_index = i * aut->nb_k;
		for(int k = 0; k < aut->nb_k; ++k)
		{
			*((double*)aut->states[i].b[k].item2) = b[t_index];
			++t_index;
		}
	}
}

int is_learning(struct automate *aut, double *init, double *a, double *b, double *prob_init, 
double *prob_a, double *prob_b)
{
	if(!prob_init || !prob_a || !prob_b)
	{
		return 0;
	}
	double diff = 0;
	double prev = 0;
	double new = 0;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		prev += init[i];
		new += prob_init[i];
	}
	diff += new - prev;
	new = 0;
	prev = 0;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		for(int j = 0; j < aut->nb_states; ++j)
		{
			prev += a[i*aut->nb_states+j];
			new += prob_a[i*aut->nb_states+j];
		}
	}	
	diff += new - prev;
	new = 0;
	prev = 0;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		for(int j = 0; j < aut->nb_k; ++j)
		{
			prev += b[i*aut->nb_k+j];
			new += prob_b[i*aut->nb_k+j];
		}
	}
	diff += new - prev;
	update_aut(aut,prob_init,prob_a,prob_b);
	return (diff/3) < 0;
}

void baum_welch(struct automate *aut, char **states, char **obs, int len, double *init, double *a, 
double *b)
{
	double *prob_init = NULL;
	double *prob_a = NULL;
	double *prob_b = NULL;
	double **alpha = NULL;
	double **beta = NULL;
	while(!is_learning(aut,init,a,b,prob_init,prob_a,prob_b))
	{
		if(prob_init && prob_a && prob_b)
		{
			free(prob_init);
			free(prob_a);
			free(prob_b);	
		}
		double ***xi_tab = calloc(sizeof(double**),len);
		double **gamma_tab = calloc(sizeof(double*),len);
		alpha = calloc(sizeof(double*),len);
		beta = calloc(sizeof(double*),len);
		forward_recursion(aut,states,len,obs,alpha);
		backward_recursion(aut,states,len,obs,beta);

		/* XI */
		for(int k = 0; k < len-1; ++k)
		{
			xi_tab[k] = calloc(sizeof(double*),aut->nb_states);
			for(int i = 0; i < aut->nb_states; ++i)
			{
				xi_tab[k][i] = calloc(sizeof(double),aut->nb_states);
				for(int j = 0; j < aut->nb_states; ++j)
				{
					xi_tab[k][i][j] = xi(aut,states,obs,len,alpha,beta,k,i,j);
					printf("%f ",xi_tab[k][i][j]);
				}
				printf("\n");
			}
			printf("\n");
		}
		printf("\n");
		
		/* GAMMA */
		for(int l = 0; l < len; ++l)
		{
			gamma_tab[l] = calloc(sizeof(double),aut->nb_states);
			for(int i = 0; i < aut->nb_states; ++i)
			{
				gamma_tab[l][i] = gamma(aut,alpha,beta,len,l,i);
				printf("%f ",gamma_tab[l][i]);
			}
			printf("\n");
		}
		printf("\n");
		for(int k = 0; k < len; ++k)
		{
			free(alpha[k]);
			free(beta[k]);
		}
		free(alpha);
		free(beta);
		prob_b = calloc(sizeof(double),aut->nb_states*aut->nb_k);
		prob_a = calloc(sizeof(double),aut->nb_states*aut->nb_states);
		prob_init = calloc(sizeof(double),aut->nb_states);	
		for(int i = 0; i < aut->nb_states; ++i)
		{
			prob_init[i] = gamma_tab[0][i]; 
			for(int j = 0; j < aut->nb_states; ++j)
			{
				double a_num = 0;
				double a_denom = 0;
				for(int l = 0; l < len-1; ++l)
				{
					a_num += xi_tab[l][i][j];
					a_denom += gamma_tab[l][i];
				}
				prob_a[i*aut->nb_states+j] = a_num/a_denom;
				printf("%f ",prob_a[i*aut->nb_states+j]);
			}
			printf("\n");
		}
		printf("\n");


		for(int i = 0; i < aut->nb_states; ++i)
		{
			for(int k = 0; k < aut->nb_k; ++k)
			{
				double b_num = 0;
				double b_denom = 0;
				for(int l = 0; l < len-1; ++l)
				{
					b_denom += gamma_tab[l][i]; 
					b_num += gamma_tab[k][i] * (obs[l] == aut->obs[k]);
				}
				prob_b[i*aut->nb_k+k] = b_num/b_denom;
				printf("%f ",prob_b[i*aut->nb_k + k]);
				printf("\n");
			}
		}
		for(int k = 0; k < len-1; ++k)
		{
			for(int i = 0; i < aut->nb_states; ++i)
			{
				free(xi_tab[k][i]);
			}
			free(xi_tab[k]);
			free(gamma_tab[k]);
		}
		free(xi_tab);
		free(gamma_tab[len-1]);
		free(gamma_tab);
	}
	free(prob_init);
	free(prob_a);
	free(prob_b);
}

void free_aut(struct automate *aut)
{
	int t_index = 0;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		t_index = i * aut->nb_states;
		for(int k = 0; k < aut->nb_states; ++k)
		{
			free(aut->states[i].a[k].item1);
			free(aut->states[i].a[k].item2);
			++t_index;
		}
		t_index = i * aut->nb_k;
		for(int k = 0; k < aut->nb_k; ++k)
		{
			free(aut->states[i].b[k].item1);
			free(aut->states[i].b[k].item2);
			++t_index;
		}
		free(aut->states[i].b);
		free(aut->states[i].a);
	}
	free(aut->states);
	free(aut);
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
	/* TEST 2 
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
	 */
	/* TEST 3 */
	int nb_states = 2;
	//Test Pluie Soleil
	int nb_obs = 20;	
	int nb_k = 3;
	//SET TRAMSITION PROBA
	char *states[] = {"LA","NY"};
	double proba_init[] = {0.6,0.4};

	char *obj[] = {"LA  ","NY  ","NULL"};

	char *obs[] = {"NULL","LA  ","LA  ","NULL","NY  ","NULL","NY  ","NY  ",
		"NY  ","NULL","NY  ","NY  ","NY  ","NY  ","NY  ","NULL","NULL","LA  ",
		"LA  ","NY  "};
	char *test_states[] = {"Hot ","Hot ","Cold","Cold"};
	//char *test_states2[] = {"Cold","Cold","Cold","Hot"};
	int test_len = 2;

	double A[] = {0.5,0.5,0.5,0.5};
	double B[] = {0.4,0.1,0.5,0.1,0.5,0.4};
	struct automate *aut = make_automate(states, obj, proba_init, A, B, nb_states,nb_obs, nb_k);

	int is_p = is_p_init_valid(aut);
	int is_t = is_t_valid(aut);
	int is_e = is_e_valid(aut);

	printf("is_p: %d\nis_t: %d\nis_e: %d\n",is_p,is_t,is_e);

	//double res = compute(aut,test_states2,obs, 2);
	//double **alpha = calloc(sizeof(double*),aut->nb_obs);
	//double **beta = calloc(sizeof(double*),aut->nb_obs);
	//double res = forward_recursion(aut,test_states,test_len,obs,alpha);
	//double res = backward_recursion(aut,test_states,test_len,obs,beta);

	char **path = malloc(aut->nb_obs*sizeof(char*));
	double res = viterbi(aut,states,test_len,obs,path);
	/*	
		for(int i = 0; i < aut->nb_obs; ++i)
		{
		printf("%s ",path[i]);
		}
		printf("\n");
	 */
	//printf("res: %f\n",res);
	baum_welch(aut,path,obs,aut->nb_obs,proba_init,A,B);
	free(path);
	free_aut(aut);
	return 0;	
}
