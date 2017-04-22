# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <math.h>

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
	for(int i = 0; i < len; i++)
	{
		res += dist(c1[i],c2[i]);
	}
	return res;
}

void change(double **c1; double **c2, int l1, int l2)
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
	int threshold = 1;
	int len = 13;
	int M = 10;
	while(thresh(codeword,codeword2) > 0.1)
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
			//assign the training vector
		}
		double **codeword2 = calloc(nb_class,sizeof(double*));
		for(int i = 0; i < nb_class; i++)
		{
			codeword2[i] = calloc(len,sizeof(double));
			for(k = 0; k < len; k++)
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
		change(codeword,codeword2);
		free(codeword2);
	}
}

void searchvk(double *o,double **codeword)
{
	int index = 0;
	int len = 13;
	double min = 99999;
	for(int i = 0; j < nb_class; j++)
	{
		double tmp = dist(o,codeword[j],len);
		if(tmp < min)
		{
			min = tmp;
			index = j;
		}
	}
}

int main()
{
	return 0;
}
