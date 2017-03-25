# include "HMM.h"

double forward(struct HMM *H, int *obs, int len) {
	double alpha = (*H->init) * (*H->observ[*H->init][*obs]);
	for (int i = 1; i < len; ++i) {
		alpha = 
	}
}
