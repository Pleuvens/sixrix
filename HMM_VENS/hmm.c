//# define _XOPEN_SOURCE 500
# include <fenv.h>
# include <math.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

# include "hmm.h"

long double get_random(int min, int max)
{
	return (long double)((rand() % (max + 1 - min)) + min);
}

int is_p_init_valid(struct automate *aut)
{
	long double sum = 0;
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
	long double sum = 0;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		for(int j = 0; j < aut->nb_states; ++j)
		{
			sum += *((long double *)aut->states[i].a[j].item2);
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
	long double sum = 0;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		for(int j = 0; j < aut->nb_k; ++j)
		{
			sum += *((long double *)aut->states[i].b[j].item2);
		}
	}
	if(sum == aut->nb_states)
	{
		return 1;
	}
	return 0;
}

struct automate* make_automate(char **states, char **k_name, long double *proba_init, 
long double *A,long double *B, int nb_states, int nb_obs, int nb_k)
{
	struct automate *aut = malloc(sizeof(struct automate));
	aut->states_n = states;
	aut->obs = k_name;
	aut->nb_states = nb_states;
	aut->len = nb_obs;
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
			aut->states[i].a[k].item2 = calloc(1,sizeof(long double));
			strncpy(aut->states[i].a[k].item1,states[k],strlen(states[k]));
			*((long double*)aut->states[i].a[k].item2) = A[t_index];
			++t_index;
		}
		t_index = i * nb_k;
		for(int k = 0; k < nb_k; ++k)
		{
			aut->states[i].b[k].item1 = calloc(strlen(k_name[k]),sizeof(char));
			aut->states[i].b[k].item2 = calloc(1,sizeof(long double));
			strncpy(aut->states[i].b[k].item1,k_name[k],strlen(k_name[k]));
			*((long double*)aut->states[i].b[k].item2) = B[t_index];
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

long double get_obs_prob(struct state *s, char *name, int nb_k)
{
	for(int i = 0; i < nb_k; ++i)
	{
		if(!strcmp(s->b[i].item1,name))
		{
			return *((long double*)(s->b[i].item2));
		}
	}
	return 0;
}

long double get_trans_prob(struct state *s, char *name, int nb_states)
{
	for(int i = 0; i < nb_states; ++i)
	{
		if(!strcmp(s->a[i].item1,name))
		{
			return *((long double*)(s->a[i].item2));
		}
	}
	return 0;
}

long double compute(struct automate *aut, char **states, char **obs, int len)
{
	long double result = 0;
	struct state *cur_state = search_state(aut,*states);
	result = cur_state->p_init * get_obs_prob(cur_state,*obs,aut->nb_k);
	//printf("%f %f ", cur_state->p_init, get_obs_prob(cur_state,*obs,aut->nb_obs));
	long double t_proba = get_trans_prob(cur_state,states[1],aut->nb_states);
	for(int i = 1; i < len; ++i)
	{
		cur_state = search_state(aut,states[i]);
		result *= t_proba * get_obs_prob(cur_state,obs[i],aut->nb_k);
		//printf("%f %f ", t_proba, get_obs_prob(cur_state,obs[i],aut->nb_obs));
		if(i < len-1)
		{
			t_proba = get_trans_prob(cur_state,states[i+1],aut->nb_states);
		}
	}
	return result;
}

double forward_recursion(struct automate *aut, char **obs, long double **alpha)
{
	struct state *cur = NULL;
	struct state *cur2 = NULL;
	long double res = 0;
	*alpha = calloc(aut->nb_states,sizeof(long double));
	long double *nk = calloc(aut->len,sizeof(long double));	

	/* INITIALISATION */
	for(int i = 0; i < aut->nb_states; ++i)
	{
		long double val = 1;
		cur = search_state(aut, *aut->states_n);
		alpha[0][i] = cur->p_init * get_obs_prob(cur,obs[0],aut->nb_k);
		for(int k = 0; k < 1; ++k) {
			for(int j = 0; j < aut->nb_states; ++j){
				cur = search_state(aut, aut->states_n[i]);
				cur2 = search_state(aut,aut->states_n[j]);
				nk[k] += cur->p_init * get_obs_prob(cur,obs[0],aut->nb_k); 
			}
			val *= (1 / nk[0]) * alpha[0][i];
		}
		alpha[0][i] = val;
		printf("%Lf ",alpha[0][i]);
	}
	printf("\n");
	/* RECURSION */
	for(int k = 1; k < aut->len; ++k)
	{
		alpha[k] = calloc(aut->nb_states,sizeof(long double));
		for(int j = 0; j < aut->nb_states; ++j)
		{
			long double val = 0;
			cur2 = search_state(aut,aut->states_n[j]);
			for(int i = 0; i < aut->nb_states; ++i)
			{
				cur = search_state(aut,aut->states_n[i]);
				val += alpha[k-1][i] * get_trans_prob(cur,aut->states_n[j],aut->nb_states);
			}
			alpha[k][j] = val * get_obs_prob(cur2, obs[k], aut->nb_k);
			val = 0;
			for(int k2 = 0; k2 < aut->nb_states; ++k2) {
				cur = search_state(aut,aut->states_n[k2]);
				for(int j2 = 0; j2 < aut->nb_states; ++j2) {
					cur2 = search_state(aut,aut->states_n[j2]);
					val += alpha[k-1][j2] * get_trans_prob(cur2,aut->states_n[k2],aut->nb_states)*
					get_obs_prob(cur,obs[k],aut->nb_k);
				}
			}
			alpha[k][j] = alpha[k][j] / val; 
			printf("%Lf ",alpha[k][j]);
		}
		printf("\n");
	}
	/* END */
	for(int i = 0; i < aut->nb_states; ++i)
	{
		res += alpha[aut->len-1][i];
	}
	return res;
}

double backward_recursion(struct automate *aut, char **obs, long double **beta)
{
	long double res = 0;
	long double val = 0;
	struct state *cur = NULL;
	beta[aut->len-1] = calloc(aut->nb_states,sizeof(long double));
	for(int i = 0; i < aut->nb_states; ++i)
	{
		beta[aut->len-1][i] = 1;
	}
	for(int k = aut->len-2; k > 0; --k)
	{
		beta[k] = calloc(aut->nb_states,sizeof(long double));
		for(int j = 0; j < aut->nb_states; ++j)
		{
			val = 0;
			for(int i = 0; i < aut->nb_states; ++i)
			{
				cur = search_state(aut, aut->states_n[i]);
				struct state *cur2 = search_state(aut,aut->states_n[j]);
				val += beta[k+1][i] * get_trans_prob(cur2,aut->states_n[i],aut->nb_states) * 
					get_obs_prob(cur,obs[k+1],aut->nb_k);
			}
			long double nk = 0;
			for(int k2 = k+1; k2 < aut->len; ++k2){
				for(int i2 = 0; i2 < aut->nb_states; ++i2) {
					nk += beta[k+1][i2];
				}
			}
			beta[k][j] = val / nk;
		}
	}
	beta[0] = calloc(aut->nb_states,sizeof(long double));
	for(int i = 0; i < aut->nb_states; ++i)
	{
		cur = search_state(aut, aut->states_n[i]);
		beta[0][i] = beta[1][i] * cur->p_init * 
			get_obs_prob(cur,obs[0],aut->nb_k);
		res += beta[0][i];
	}
	return res;
}

double _max(long double a,long double b)
{
	if(a > b)
	{
		return a;
	} else {
		return b;
	}
}

double viterbi(struct automate *aut, char **states, char **obs,char **path)
{
	struct state *cur = search_state(aut, aut->states_n[0]);
	struct state *cur2 = NULL;
	long double **max = calloc(aut->len,sizeof(long double*));
	int **tab_path = calloc(aut->len,sizeof(int*));
	*max = calloc(aut->nb_states,sizeof(long double));
	*tab_path = calloc(aut->nb_states,sizeof(int));
	long double res = 0;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		cur = search_state(aut, aut->states_n[i]);
		max[0][i] = cur->p_init * get_obs_prob(cur,obs[0],aut->nb_k);
		//printf("obs: %Lf",get_obs_prob(cur,obs[0],aut->nb_k));
		tab_path[0][i] = -1;
	}
	for(int k = 1; k < aut->len; ++k)
	{
		max[k] = calloc(aut->nb_states,sizeof(long double));
		tab_path[k] = calloc(aut->nb_states,sizeof(int));
		for(int j = 0; j < aut->nb_states; ++j)
		{
			cur2 = search_state(aut,aut->states_n[j]);
			long double val_max = 0;
			for(int i = 0; i < aut->nb_states; ++i)
			{
				cur = search_state(aut,aut->states_n[i]);
				long double tmp = get_trans_prob(cur,aut->states_n[j],aut->nb_states) * 
					get_obs_prob(cur2,obs[k],aut->nb_k) * max[k-1][i];
				val_max = _max(val_max, tmp);
			}
			max[k][j] = val_max;
			/*PATH*/
			for(int i = 0; i < aut->nb_states; ++i)
			{
				cur = search_state(aut,aut->states_n[i]);
				long double tmp = get_trans_prob(cur,aut->states_n[j],aut->nb_states) * 
					get_obs_prob(cur2,obs[k],aut->nb_k) * max[k-1][i];
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
		res = _max(res,max[aut->len-1][i]);
	}
	/* TEST Backtrack */
	int previous = -1;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		if(max[aut->len-1][i] == res)
		{
			path[aut->len-1] = aut->states_n[i];
			previous = i;
			break;
		}
	}
	for(int k = aut->len-2; k > -1; --k)
	{
		path[k] = states[tab_path[k+1][previous]];
		previous = tab_path[k+1][previous];
	}

	for(int k = 0; k < aut->len-1; ++k)
	{
		free(tab_path[k]);
	}
	free(tab_path);
	free(max[aut->len-1]);
	free(max);
	return res;
}

long double xi(struct automate *aut, char **obs, long double **alpha, long double **beta,
		int k, int index_i, int index_j)
{
	struct state *cur_i = NULL;
	struct state *cur_j = NULL;
	/* NUMERATEUR */
	cur_i = search_state(aut,aut->states_n[index_i]);
	cur_j = search_state(aut,aut->states_n[index_j]);
	long double numerateur =  alpha[k][index_i]  * get_trans_prob(cur_i,aut->states_n[index_j],
			aut->nb_states) * get_obs_prob(cur_j,obs[k+1],aut->nb_k) *  beta[k+1][index_j];
	/* DENOMINATEUR */ /* FIXME */

	long double denominateur = beta[k][index_i];
	/*for(int i = 0; i < aut->nb_states; ++i)
	{
		cur_i = search_state(aut,aut->states_n[i]);
		for(int j = 0; j < aut->nb_states; ++j)
		{
			cur_j = search_state(aut,aut->states_n[j]);
			denominateur +=  alpha[k][i] * get_trans_prob(cur_i,aut->states_n[j],aut->nb_states) *
				get_obs_prob(cur_j,obs[k+1],aut->nb_k) * beta[k+1][j];
		}
	}*/

	return numerateur/denominateur;
}

long double gamma_(struct automate *aut, long double **alpha, long double **beta, int k, int index)
{
	/* NUMERATEUR */
	long double numerateur = alpha[k][index] * beta[k][index];

	/* DENOMINATEUR */
	long double denominateur = 0;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		denominateur += alpha[k][i] * beta[k][i];
	}
	return numerateur / denominateur;
}

void update_aut(struct automate *aut, long double *init, long double *a, long double *b)
{
	int t_index = 0;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		aut->states[i].p_init = init[i];
		t_index = i * aut->nb_states;
		for(int k = 0; k < aut->nb_states; ++k)
		{
			*((long double*)aut->states[i].a[k].item2) = a[t_index];
			++t_index;
		}
		t_index = i * aut->nb_k;
		for(int k = 0; k < aut->nb_k; ++k)
		{
			*((long double*)aut->states[i].b[k].item2) = b[t_index];
			++t_index;
		}
	}
}

int is_learning(struct automate *aut, long double *init, long double *a, long double *b, long double *prob_init, 
long double *prob_a, long double *prob_b)
{
	if(!prob_init || !prob_a || !prob_b)
	{
		return 0;
	}
	long double diff = 0;
	long double prev = 0;
	long double new = 0;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		if(prev < 999) {
			prev += init[i];
		}
		if(new < 999) {
			new += prob_init[i];
		}
	}
	diff += new - prev;
	new = 0;
	prev = 0;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		for(int j = 0; j < aut->nb_states; ++j)
		{
			if(prev < 999)
			{
				prev += a[i*aut->nb_states+j];
			}
			if(new < 999)
			{
				new += prob_a[i*aut->nb_states+j];
			}
		}
	}	
	diff += new - prev;
	new = 0;
	prev = 0;
	for(int i = 0; i < aut->nb_states; ++i)
	{
		for(int j = 0; j < aut->nb_k; ++j)
		{
			if(prev < 999) {
				prev += b[i*aut->nb_k+j];
			}
			if(new < 999) {
				new += prob_b[i*aut->nb_k+j];
			}
		}
	}
	diff += new - prev;
	if(diff < 0) {
		diff *= -1;
	}
	//printf("prev: %Lf\nnew: %Lf\n",prev,new);
	update_aut(aut,prob_init,prob_a,prob_b);
	//printf("%Lf\n",diff/3);
	return (diff/3) <= 1;
}

void baum_welch(struct automate *aut, char **obs, 
		long double *prob_init,long double *prob_a,long double *prob_b)
{	
	   long double **alpha = NULL;
	   long double **beta = NULL;
	   long double ***xi_tab = calloc(sizeof(long double**),aut->len);
	   long double **gamma_tab = calloc(sizeof(long double*),aut->len);
	   alpha = calloc(sizeof(long double*),aut->len);
	   beta = calloc(sizeof(long double*),aut->len);
	   forward_recursion(aut,obs,alpha);
	   backward_recursion(aut,obs,beta);

	   /* XI */
	   for(int k = 0; k < aut->len-1; ++k)
	   {
		   xi_tab[k] = calloc(sizeof(long double*),aut->nb_states);
		   for(int i = 0; i < aut->nb_states; ++i)
		   {
			   xi_tab[k][i] = calloc(sizeof(long double),aut->nb_states);
			   for(int j = 0; j < aut->nb_states; ++j)
			   {
				   xi_tab[k][i][j] = xi(aut,obs,alpha,beta,k,i,j);
				   //printf("%f ",xi_tab[k][i][j]);
			   }
			   //printf("\n");
		   }
		   //printf("\n");
	   }
	   //printf("\n");

	   /* GAMMA */
	   for(int l = 0; l < aut->len; ++l)
	   {
		   gamma_tab[l] = calloc(sizeof(long double),aut->nb_states);
		   for(int i = 0; i < aut->nb_states; ++i)
		   {
			   gamma_tab[l][i] = gamma_(aut,alpha,beta,l,i);
			   //printf("%f ",gamma_tab[l][i]);
		   }
		   //printf("\n");
	   }
	   //printf("\n");
	   for(int k = 0; k < aut->len; ++k)
	   {
		   free(alpha[k]);
		   free(beta[k]);
	   }
	   free(alpha);
	   free(beta);
	   for(int i = 0; i < aut->nb_states; ++i)
	   {
		   prob_init[i] = gamma_tab[0][i]; 
		   for(int j = 0; j < aut->nb_states; ++j)
		   {
			   long double a_num = 0;
			   long double a_denom = 0;
			   for(int l = 0; l < aut->len-1; ++l)
			   {
				   a_num += xi_tab[l][i][j];
				   a_denom += gamma_tab[l][i];
			   }
			   prob_a[i*aut->nb_states+j] = a_num/a_denom;
			   //printf("%f ",prob_a[i*aut->nb_states+j]);
		   }
		   //printf("\n");
	   }
	   //printf("\n");


	   for(int i = 0; i < aut->nb_states; ++i)
	   {
		   for(int k = 0; k < aut->nb_k; ++k)
		   {
			   long double b_num = 0;
			   long double b_denom = 0;
			   for(int l = 0; l < aut->len-1; ++l)
			   {
				   b_denom += gamma_tab[l][i]; 
				   b_num += gamma_tab[k][i] * (obs[l] == aut->obs[k]);
			   }
			   prob_b[i*aut->nb_k+k] = b_num/b_denom;
			   //printf("%f ",prob_b[i*aut->nb_k + k]);
			   //printf("\n");
		   }
	   }
	   for(int k = 0; k < aut->len-1; ++k)
	   {
		   for(int i = 0; i < aut->nb_states; ++i)
		   {
			   free(xi_tab[k][i]);
		   }
		   free(xi_tab[k]);
		   free(gamma_tab[k]);
	   }
	   free(xi_tab);
	   free(gamma_tab[aut->len-1]);
	   free(gamma_tab);
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

void saveVal_(struct automate *aut)
{
	char *path = "./save";
	FILE *f = fopen(path,"w");
	//int t_index = 0;
	fprintf(f,"%d\n",aut->nb_states);
	fprintf(f,"%d\n",aut->nb_k);

	for(int i = 0; i < aut->nb_states; ++i) {
		fprintf(f,"%Lf ", aut->states[i].p_init);
	}

	for(int i = 0; i < aut->nb_states; ++i)
	{
		//t_index = i * aut->nb_states;
		for(int k = 0; k < aut->nb_states; ++k)
		{
			fprintf(f,"%Lf ", *(long double*)(aut->states[i].a[k].item2));
		}
		//fprintf("\n");
	}
	for(int i = 0; i < aut->nb_states; ++i)
	{
		//t_index = i * aut->nb_states;
		for(int k = 0; k < aut->nb_k; ++k)
		{
			fprintf(f,"%Lf ", *(long double*)(aut->states[i].b[k].item2));
		}
		//fprintf("\n");
	}
	fclose(f);
}

void loadVal_(long double **proba, long double **A, long double **B, int *nb_states, int *nb_k)
{
	char *path = "";
	FILE *f = fopen(path,"r");

	fscanf(f,"%d",nb_states);
	fscanf(f,"%d",nb_k);

	for(int i = 0; i < *nb_states; ++i)
	{
		fscanf(f,"%Lf",proba[i]);
	}

	for(int i = 0; i < *nb_states; ++i)
	{
		//t_index = i * aut->nb_states;
		for(int k = 0; k < *nb_states; ++k)
		{
			fscanf(f,"%Lf", A[i * *nb_states + k]);
		}
		//fprintf("\n");
	}
	for(int i = 0; i < *nb_states; ++i)
	{
		//t_index = i * aut->nb_states;
		for(int k = 0; k < *nb_k; ++k)
		{
			fscanf(f,"%Lf", B[i * *nb_states + k]);
		}
		//fprintf("\n");
	}
	fclose(f);
}

void copy_init(long double *src, long double *dst,int len) {
	for(int i = 0; i < len; ++i)
	{
		dst[i] = src[i];
	}
}

void learning(struct automate *aut, char ***states, char **obs, long double *proba_init,
long double *A, long double *B, int training_len)
{
	long double *prob_init = NULL;
	long double *prob_a = NULL;
	long double *prob_b = NULL;
	long double *a = calloc(aut->nb_states*aut->nb_states,sizeof(long double));
	copy_init(A,a,aut->nb_states*aut->nb_states);
	long double *b = calloc(aut->nb_states*aut->nb_k,sizeof(long double));
	copy_init(B,b,aut->nb_states*aut->nb_k);
	long double *p_init = calloc(aut->nb_states,sizeof(long double));
	copy_init(proba_init,p_init,aut->nb_states);
	while(!is_learning(aut,p_init,a,b,prob_init,prob_a,prob_b))
	{	
		for(int i = 0; i < training_len; ++i)
		{
			if(prob_init && prob_a)
			{	
				free(p_init);
				free(a);
				free(b);
				p_init = calloc(aut->nb_states,sizeof(long double));
				copy_init(prob_init,p_init,aut->nb_states);
				a = calloc(aut->nb_states*aut->nb_states,sizeof(long double));
				copy_init(prob_a,a,aut->nb_states*aut->nb_states);
				b = calloc(aut->nb_states*aut->nb_k,sizeof(long double));
				copy_init(prob_b,b,aut->nb_states*aut->nb_k);
				prob_init = NULL;
				prob_a = NULL;
				prob_b = NULL;	
			}	
			char **path = malloc(aut->len*sizeof(char*));
			viterbi(aut,states[i],obs,path);
			prob_b = calloc(sizeof(long double),aut->nb_states*aut->nb_k);
			prob_a = calloc(sizeof(long double),aut->nb_states*aut->nb_states);
			prob_init = calloc(sizeof(long double),aut->nb_states);
			baum_welch(aut,obs,prob_init,prob_a,
					prob_b);
			is_learning(aut,p_init,a,b,prob_init,prob_a,prob_b);
			free(path);
		}
	}
	free(prob_init);
	free(prob_a);
	free(prob_b);
	saveVal_(aut);
}
void mainHMM(char **obs)
{
	feenableexcept(FE_ALL_EXCEPT & ~FE_INEXACT);
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
	int nb_states = 4;
	//Test Pluie Soleil
	extern long frameNbr_;
	int nb_obs = (int)frameNbr_;	
	int nb_k = 5;
	//SET TRAMSITION PROBA
	/*
	   char *states[] = {"ay  ","aww ","ahh ","aah ","eh  ","ee  ","d   ","aye ","ff  ","ewe ","ss  ","tt  ","qu  ","rr  ","jj  ","ih  ","hh  ","g   ",
	   "nn  ","mm  ","ll  ","k   ","pp  ","ohh ","err ","ch  ","au  ","arr ","oww ","ooo ","nng ","hw  ","shh ","oy  ","uh  ","oo  ","th  ","zzz ","vvv ",
	   "kss ","yyuh","zh  ","wuh "};

	   double proba_init[] = {0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395};
	 */
	char *states[] = {"ohh","pp ", "eh ", "nn "};
	long double proba_init[] = {0.25,0.25,0.25,0.25};

	char *obj[] = {"c1","c2","c3","c4","null"};

	//char *obs[] = {"ohh","pp ","eh ","nn "};
	//char *test_states[] = {"Hot ","Hot ","Cold","Cold"};
	//char *test_states2[] = {"Cold","Cold","Cold","Hot"};
	//int test_len = 2;
	long double A[] = {0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25};
	/*
	   double A[] = {0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	   0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
		0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,0.02325581395,
	}; */
	long double B[] = {0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,
	0.2,0.2,0.2,0.2,0.2};
	struct automate *aut = make_automate(states, obj, proba_init, A, B, nb_states,nb_obs, nb_k);

	char ***s = malloc(sizeof(char**));
	s[0] = states;
	learning(aut,s,obs,proba_init,A,B,1);
	char **path = malloc(aut->len*sizeof(char*));
	viterbi(aut,states,obs,path);
	for(int i = 0; i < aut->len; i++) {
		printf("%s ",path[i]);
	}
	printf("\n");
	free_aut(aut);
	free(s);	
}
