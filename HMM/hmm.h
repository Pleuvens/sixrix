# ifndef HMM_H
# define HMM_H

struct tuple {
	void *item1;
	void *item2;
};

struct state {
	char *name;
	double p_init; //Probability of being the initial state
	struct tuple *a; //Probability of the transition between 2 states 
	struct tuple *b; //Probavility of containing element k
};

struct automate {
	int nb_states;
	struct state *states;	
};

#endif
