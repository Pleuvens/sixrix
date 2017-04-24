# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <math.h>

# include "../mfcc/mfcc.c"

double dist(double *x, double *y, int len)
{
	double res = 0;
	for(int i = 0; i < len; i++)
	{
		res += x[i] * x[i] - y[i] * y[i];
	}
	return sqrt(res);
}

double thresh(double **c1, double **c2, int len)
{
	if(!c2)
	{
		return 1.0;
	}
	double res = 0;
	int len2 = 13;
	for(int i = 0; i < len; i++)
	{
		res += dist(c1[i],c2[i],len2);
	}
	return res;
}

void change(double **c1, double **c2, int l1, int l2)
{
	for(int i = 0; i < l1; ++i)
	{
		for(int j = 0; j < l2; ++j)
		{
			c1[i][j] = c2[i][j];
		}
		free(c2[i]);
	}
}

void generate(double **vects, double **codeword)
{
	int len = 13;
	int M = 1;
	int nb_class = 1;
	double **codeword2 = NULL;
	int *vertex_by_class = calloc(nb_class,sizeof(int));
	while(thresh(codeword,codeword2,nb_class) > 0.1)
	{
		for(int i = 0; i < M; i++)
		{
			int index = 0;
			double min = 9999999;
			for(int j = 0; j < nb_class; j++)
			{
				double tmp = dist(vects[i],codeword[j],len);
				if(tmp < min)
				{
					min = tmp;
					index = j;
				}
			}
			vertex_by_class[index] += 1;
		}
		codeword2 = calloc(nb_class,sizeof(double*));
		for(int i = 0; i < nb_class; i++)
		{
			codeword2[i] = calloc(len,sizeof(double));
			for(int k = 0; k < len; k++)
			{
				double num = 0;
				double denom = 0;
				for(int j = 0; j < M; ++j)
				{
					num = vects[i][j] * codeword[i][j];
					denom = codeword[i][j];
				}
				codeword2[i][k] = num/denom;
			}
		}
		change(codeword,codeword2,M,len);
		free(codeword2);
	}
}

int searchvk(double *o,double **codeword, int nb_class)
{
	int index = 0;
	int len = 13;
	double min = 99999;
	for(int i = 0; i < nb_class; i++)
	{
		double tmp = dist(o,codeword[i],len);
		if(tmp < min)
		{
			min = tmp;
			index = i;
		}
	}
	return index;
}

void saveVal(double **codeword,int l1, int l2)
{
	char *path = "./vqsave";
	FILE *f = fopen(path,"w");
	
	fprintf(f,"%d\n",l1);
	fprintf(f,"%d\n",l2);

	for(int i = 0; i < l1; i++)
	{
		for(int j = 0; j < l2; j++)
		{
			fprintf(f,"%f ", codeword[i][j]);
		}
	}
	fclose(f);
}

void loadVal(double **codeword, int *l1, int *l2)
{
	char *path = "./vqsave";
	FILE *f = fopen(path,"r");
	
	fscanf(f,"%d",l1);
	fscanf(f,"%d",l2);
	codeword = calloc(*l1,sizeof(double*));
	for(int i = 0; i < *l1; i++)
	{
		codeword[i] = calloc(*l2,sizeof(double));
		for(int j = 0; j < *l2; j++)
		{
			fscanf(f,"%lf",codeword[i]+j);
		}
	}
}

char* state_match(int index)
{
	int val = index + 3;
	index = val;
	//FIXME
	return "a";
}

void randomCW(double **cw)
{
	for(int i = 0; i < 2; i++)
	{
		cw[i] = calloc(13,sizeof(double));
		for(int j = 0; j < 13; j++)
		{
			cw[i][j] = (double)(99*(double)rand()/RAND_MAX);
		}
	}
}

char **mainProcess(double **o)
{
	//Load file
	int nb_class = 1;
	int len = 13;
	extern long frameNbr_;
	int len_o = (int)frameNbr_;
	double **codeword = NULL;	
	FILE *f = NULL;
	if((f = fopen("./vqsave","r")))
	{
		fclose(f);
		loadVal(codeword,&nb_class,&len);
	} else {
		codeword = calloc(nb_class,sizeof(double*));	
		randomCW(codeword);	
		generate(o,codeword);
		saveVal(codeword,nb_class,len);
	}
	char **res = calloc(len_o,sizeof(char*));
	for(int i = 0; i < len_o; i++)
	{
		int index = searchvk(o[i],codeword,nb_class);
		char *class = state_match(index);
		res[i] = calloc(strlen(class),sizeof(char));
		res[i] = strcpy(res[i],class);
	}
	return res;
}

int main()
{
	/*
	OPEN_MAL.WAV
	193319__margo-heston__oo.wav
	193284__margo-heston__pp.wav
	193266__margo-heston__eh.wav
	193280__margo-heston__nn.wav
	*/
	double **oo = MFCC("../phonemes/193319__margo-heston__oo.wav");
	mainProcess(oo);
	/*
	double **pp = MFCC("193284__margo-heston__pp.wav");
	double **eh = MFCC("193266__margo-heston__eh.wav");
	double **nn = MFCC("193280__margo-heston__nn.wav");
	*/
	return 0;
}
