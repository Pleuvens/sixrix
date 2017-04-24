# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <math.h>

# include "../mfcc/mfcc.c"

double dist(double *x, double y, int len)
{
	double res = 0;
	for(int i = 0; i < len; i++)
	{
		res += x[i] - y * x[i] - y;
	}
	return sqrt(res);
}

double thresh(double *c1, double *c2, int len)
{
	if(!c2)
	{
		return 1.0;
	}
	double res = 0;
	for(int i = 0; i < len; i++)
	{
		double tmp = c1[i] - c2[i];
		if(tmp < 0)
		{
			tmp *= -1;
		}
		res += tmp;
	}
	return res;
}

void change(double *c1, double *c2, int l1)
{
	for(int i = 0; i < l1; ++i)
	{
		c1[i] = c2[i];		
	}
}

void generate(double ***vects, double *codeword, long *len_o)
{
	int len = 13;
	int M = 4;
	int nb_class = 2;
	double *codeword2 = NULL;
	int *vertex_by_class = calloc(nb_class,sizeof(int));
	while(thresh(codeword,codeword2,nb_class) > 0.1)
	{
		for(int i = 0; i < M; i++)
		{
			for(long k = 0; k < len_o[M]; ++k)
			{
				int index = 0;
				double min = 9999999;
				for(int j = 0; j < nb_class; j++)
				{
					double tmp = dist(vects[i][k],codeword[j],len);
					if(tmp < min)
					{
						min = tmp;
						index = j;
					}
				}
				vertex_by_class[index] += 1;
			}
		}
		codeword2 = calloc(nb_class,sizeof(double*));
		/*for(int i = 0; i < nb_class; i++)
		{
			codeword2[i] = calloc(len,sizeof(double));
			for(int k = 0; k < len; k++)
			{
				double num = 0;
				double denom = 0;
				for(int j = 0; j < M; ++j)
				{
					num = vects  codeword[i][j];
					denom = codeword[i][j];
				}
				codeword2[i][k] = num/denom;
			}
		}*/
		for(int j = 0; j < nb_class; ++j)
		{
			//codeword2[j] = calloc(len,sizeof(double));
			double num = 0;
			double denom = 0;
			for(int i = 0; i < M; ++i)
			{
				for(int k = 0; k < len_o[M]; ++k)
				{
					for(int n = 0; n < len; ++n)
					{
						num += vertex_by_class[i] * vects[i][k][n];
					}
					denom += vertex_by_class[i];
				}
			}
			codeword[j] = num/denom;
		}
		change(codeword,codeword2,nb_class);
		free(codeword2);
	}
}

int searchvk(double *o,double *codeword, int nb_class)
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

void saveVal(double *codeword,int l1)
{
	char *path = "./vqsave";
	FILE *f = fopen(path,"w");
	
	fprintf(f,"%d\n",l1);

	for(int i = 0; i < l1; i++)
	{
		fprintf(f,"%f ", codeword[i]);
	}
	fclose(f);
}

void loadVal(double *codeword, int *l1)
{
	char *path = "./vqsave";
	FILE *f = fopen(path,"r");
	
	fscanf(f,"%d",l1);
	codeword = calloc(*l1,sizeof(double*));
	for(int i = 0; i < *l1; i++)
	{
		fscanf(f,"%lf",codeword+i);
	}
}

char* state_match(int index)
{
	char *ph = NULL;
	switch(index)
	{
		case 0:
			ph = "oo";
			break;
		default:
			ph = "null";
			break;
	}
	return ph;
}

void randomCW(double *cw)
{
	for(int i = 0; i < 2; i++)
	{
		cw[i] = (double)(99*(double)rand()/RAND_MAX);
	}
}

char **mainProcess(double **o, long len)
{
	//Load file
	int nb_class = 2;
	extern long frameNbr_;
	long *len_o = calloc(nb_class,sizeof(long));
	double *codeword = NULL;	
	FILE *f = NULL;
	if((f = fopen("./vqsave","r")))
	{
		fclose(f);
		loadVal(codeword,&nb_class);
	} else {
		codeword = calloc(nb_class,sizeof(double*));	
		randomCW(codeword);	
		double ***vects = calloc(4,sizeof(double**));	
		vects[0] = MFCC("../phonemes/193319__margo-heston__oo.wav");
		len_o[0] = frameNbr_;
		vects[1] = MFCC("../phonemes/193284__margo-heston__pp.wav");
		len_o[1] = frameNbr_;
		vects[2] = MFCC("../phonemes/193266__margo-heston__eh.wav");
		len_o[2] = frameNbr_;
		vects[3] = MFCC("../phonemes/193280__margo-heston__nn.wav");
		len_o[3] = frameNbr_;
		generate(vects,codeword,len_o);
		saveVal(codeword,nb_class);
	}
	char **res = calloc(len,sizeof(char*));
	for(long i = 0; i < len; i++)
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
	extern long frameNbr_;
	double **oo = MFCC("../phonemes/193319__margo-heston__oo.wav");
	long l = frameNbr_;
	char **r = mainProcess(oo,l);
	for(int i = 0; i < l; ++i)
	{
		printf("%s\n",r[i]);
	}
	/*
	double **pp = MFCC("193284__margo-heston__pp.wav");
	double **eh = MFCC("193266__margo-heston__eh.wav");
	double **nn = MFCC("193280__margo-heston__nn.wav");
	*/
	return 0;
}
