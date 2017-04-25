# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <math.h>
# include <time.h>

# include "../mfcc/mfcc.c"

long long dist(double *x, double *y, int len)
{
	long long res = 0;
	for(int i = 0; i < len; i++)
	{
		res += x[i] - y[i] * x[i] - y[i];
	}
	if(res < 0)
	{
		res *= -1;
	}
	res = sqrt(res);	
	return res;
}

double thresh(double **c1, double **c2, int len, int len2)
{
	if(!c2)
	{
		return 1.0;
	}
	double res = 0;
	for(int i = 0; i < len; i++)
	{
		for(int j = 0; j < len2; ++j)
		{
			double tmp = c1[i][j] - c2[i][j];
			/*
			if(tmp < 0)
			{
				tmp *= -1;
			}
			*/
			res += tmp;
		}
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
	free(c2);
}

void generate(double ***vects, double **codeword, long *len_o)
{
	int len = 13;
	int M = 4;
	int nb_class = 43;
	double **codeword2 = NULL;
	int *vertex_by_class = calloc(nb_class,sizeof(int));
	while(thresh(codeword,codeword2,nb_class,len) > 1)
	{
		for(int i = 0; i < M; i++)
		{
			for(long k = 0; k < len_o[i]; ++k)
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
			codeword2[j] = calloc(len,sizeof(double));
			double num = 0;
			double denom = 0;
			for(int i = 0; i < M; ++i)
			{
				for(int k = 0; k < len_o[i]; ++k)
				{
					for(int n = 0; n < len; ++n)
					{
						num += vertex_by_class[i] * vects[i][k][n];
					}
					denom += vertex_by_class[i];
				}
				codeword[j][i] = num/denom;
			}
		}
		change(codeword,codeword2,nb_class,len);
	}
}

int searchvk(double *o,double **codeword, int nb_class)
{
	int index = 0;
	int len = 13;
	double min = 99999;
	for(int i = 0; i < nb_class; i++)
	{
		long long tmp = dist(o,codeword[i],len);
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

	for(int i = 0; i < l1; i++)
	{
		for(int j = 0; j < l2; ++j)
		{
			fprintf(f,"%f ", codeword[i][j]);
		}
	}
	fclose(f);
}

double** loadVal(double **codeword, int *l1, int l2)
{
	char *path = "./vqsave";
	FILE *f = fopen(path,"r");
	
	fscanf(f,"%d",l1);
	codeword = calloc(*l1,sizeof(double*));
	for(int i = 0; i < *l1; i++)
	{
		codeword[i] = calloc(l2,sizeof(double));
		for(int j = 0; j < l2; ++j)
		{
			fscanf(f,"%lf",codeword[i]+j);
		}
	}
	fclose(f);
	return codeword;
}

char* state_match(int index)
{
	char *ph = NULL;
	switch(index)
	{
		case 0:
			ph = "ay";
			break;
		case 1:
			ph = "aww";
			break;
		case 2:
			ph = "ahh";
			break;
		case 3:
			ph = "aah";
			break;
		case 4:
			ph = "eh";
			break;
		case 5:
			ph = "ee";
			break;
		case 6:
			ph = "d";
			break;
		case 7:
			ph = "aye";
			break;
		case 8:
			ph = "ff";
			break;
		case 9:
			ph = "ewe";
			break;
		case 10:
			ph = "ss";
			break;
		case 11:
			ph = "tt";
			break;
		case 12:
			ph = "qu";
			break;
		case 13:
			ph = "rr";
			break;
		case 14:
			ph = "jj";
			break;
		case 15:
			ph = "ih";
			break;
		case 16:
			ph = "hh";
			break;
		case 17:
			ph = "g";
			break;
		case 18:
			ph = "nn";
			break;
		case 19:
			ph = "mm";
			break;
		case 20:
			ph = "ll";
			break;
		case 21:
			ph = "k";
			break;
		case 22:
			ph = "pp";
			break;
		case 23:
			ph = "ohh";
			break;
		case 24:
			ph = "err";
			break;
		case 25:
			ph = "ch";
			break;
		case 26:
			ph = "au";
			break;
		case 27:
			ph = "arr";
			break;
		case 28:
			ph = "oww";
			break;
		case 29:
			ph = "ooo";
			break;
		case 30:
			ph = "nng";
			break;
		case 31:
			ph = "hw";
			break;
		case 32:
			ph = "shh";
			break;
		case 33:
			ph = "oy";
			break;
		case 34:
			ph = "uh";
			break;
		case 35:
			ph = "oo";
			break;	
		case 36:
			ph = "th";
			break;
		case 37:
			ph = "zzz";
			break;
		case 38:
			ph = "vvv";
			break;
		case 39:
			ph = "kss";
			break;	
		case 40:
			ph = "yyuh";
			break;
		case 41:
			ph = "zh";
			break;
		case 42:
			ph = "eh";
			break;
		default:
			ph = "null";
			break;
	}
	return ph;
}

void randomCW(double **cw, int nb_class, int len)
{
	for(int i = 0; i < nb_class; i++)
	{
		cw[i] = calloc(len,sizeof(double));
		for(int j = 0; j < len; ++j)
		{
			cw[i][j] = (double)(rand() % 490 + (-40));
		}
	}
}

char **mainProcess(double **o, long len)
{
	//Load file
	int nb_class = 43;
	extern long frameNbr_;
	long *len_o = calloc(nb_class,sizeof(long));
	double **codeword = NULL;	
	FILE *f = NULL;
	if((f = fopen("./vqsave","r")))
	{
		fclose(f);
		codeword = loadVal(codeword,&nb_class,13);
	} else {
		codeword = calloc(nb_class,sizeof(double*));	
		randomCW(codeword,nb_class,13);	
		double ***vects = calloc(nb_class,sizeof(double**));	
		vects[0] = MFCC("../phonemes/193262__margo-heston__ay.wav");
		len_o[0] = frameNbr_;
		vects[1] = MFCC("../phonemes/193263__margo-heston__aww.wav");
		len_o[1] = frameNbr_;
		vects[2] = MFCC("../phonemes/193264__margo-heston__ahh.wav");
		len_o[2] = frameNbr_;
		vects[3] = MFCC("../phonemes/193265__margo-heston__aah.wav");
		len_o[3] = frameNbr_;
		vects[4] = MFCC("../phonemes/193266__margo-heston__eh.wav");
		len_o[4] = frameNbr_;
		vects[5] = MFCC("../phonemes/193267__margo-heston__ee.wav");
		len_o[5] = frameNbr_;
		vects[6] = MFCC("../phonemes/193268__margo-heston__d.wav");
		len_o[6] = frameNbr_;
		vects[7] = MFCC("../phonemes/193269__margo-heston__aye.wav");
		len_o[7] = frameNbr_;
		vects[8] = MFCC("../phonemes/193270__margo-heston__ff.wav");
		len_o[8] = frameNbr_;
		vects[9] = MFCC("../phonemes/193271__margo-heston__ewe.wav");
		len_o[9] = frameNbr_;
		vects[10] = MFCC("../phonemes/193272__margo-heston__ss.wav");
		len_o[10] = frameNbr_;
		vects[11] = MFCC("../phonemes/193273__margo-heston__tt.wav");
		len_o[11] = frameNbr_;
		vects[12] = MFCC("../phonemes/193274__margo-heston__qu.wav");
		len_o[12] = frameNbr_;
		vects[13] = MFCC("../phonemes/193275__margo-heston__rr.wav");
		len_o[13] = frameNbr_;
		vects[14] = MFCC("../phonemes/193276__margo-heston__jj.wav");
		len_o[14] = frameNbr_;
		vects[15] = MFCC("../phonemes/193277__margo-heston__ih.wav");
		len_o[15] = frameNbr_;
		vects[16] = MFCC("../phonemes/193278__margo-heston__hh.wav");
		len_o[16] = frameNbr_;
		vects[17] = MFCC("../phonemes/193279__margo-heston__g.wav");
		len_o[17] = frameNbr_;
		vects[18] = MFCC("../phonemes/193280__margo-heston__nn.wav");
		len_o[18] = frameNbr_;
		vects[19] = MFCC("../phonemes/193281__margo-heston__mm.wav");
		len_o[19] = frameNbr_;
		vects[20] = MFCC("../phonemes/193282__margo-heston__ll.wav");
		len_o[20] = frameNbr_;
		vects[21] = MFCC("../phonemes/193283__margo-heston__k.wav");
		len_o[21] = frameNbr_;
		vects[22] = MFCC("../phonemes/193284__margo-heston__pp.wav");
		len_o[22] = frameNbr_;
		vects[23] = MFCC("../phonemes/193285__margo-heston__ohh.wav");
		len_o[23] = frameNbr_;
		vects[24] = MFCC("../phonemes/193304__margo-heston__err.wav");
		len_o[24] = frameNbr_;
		vects[25] = MFCC("../phonemes/193305__margo-heston__ch.wav");
		len_o[25] = frameNbr_;
		vects[26] = MFCC("../phonemes/193306__margo-heston__au.wav");
		len_o[26] = frameNbr_;
		vects[27] = MFCC("../phonemes/193307__margo-heston__arr.wav");
		len_o[27] = frameNbr_;
		vects[28] = MFCC("../phonemes/193308__margo-heston__oww.wav");
		len_o[28] = frameNbr_;
		vects[29] = MFCC("../phonemes/193309__margo-heston__ooo.wav");
		len_o[29] = frameNbr_;
		vects[30] = MFCC("../phonemes/193310__margo-heston__nng.wav");
		len_o[30] = frameNbr_;
		vects[31] = MFCC("../phonemes/193311__margo-heston__hw.wav");
		len_o[31] = frameNbr_;
		vects[32] = MFCC("../phonemes/193312__margo-heston__shh.wav");
		len_o[32] = frameNbr_;
		vects[33] = MFCC("../phonemes/193313__margo-heston__oy.wav");
		len_o[33] = frameNbr_;
		vects[34] = MFCC("../phonemes/193318__margo-heston__uh.wav");
		len_o[34] = frameNbr_;
		vects[35] = MFCC("../phonemes/193319__margo-heston__oo.wav");
		len_o[35] = frameNbr_;
		vects[36] = MFCC("../phonemes/193320__margo-heston__th.wav");
		len_o[36] = frameNbr_;
		vects[37] = MFCC("../phonemes/193321__margo-heston__zzz.wav");
		len_o[37] = frameNbr_;
		vects[38] = MFCC("../phonemes/193322__margo-heston__vvv.wav");
		len_o[38] = frameNbr_;
		vects[39] = MFCC("../phonemes/193323__margo-heston__kss.wav");
		len_o[39] = frameNbr_;
		vects[40] = MFCC("../phonemes/193324__margo-heston__yyuh.wav");
		len_o[40] = frameNbr_;
		vects[41] = MFCC("../phonemes/193325__margo-heston__zh.wav");
		len_o[41] = frameNbr_;
		vects[42] = MFCC("../phonemes/193326__margo-heston__wuh.wav");
		len_o[42] = frameNbr_;
		generate(vects,codeword,len_o);
		saveVal(codeword,nb_class,13);
	}
	char **res = calloc(len,sizeof(char*));
	for(long i = 0; i < len; i++)
	{
		int index = searchvk(o[i],codeword,nb_class);
		char *class = state_match(index);
		res[i] = calloc(strlen(class)+1,sizeof(char));
		res[i] = strcpy(res[i],class);
	}
	return res;
}

int main()
{
	srand(time(NULL));
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
