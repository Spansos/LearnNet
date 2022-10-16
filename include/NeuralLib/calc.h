#ifndef NN_CALC_H
#define NN_CALC_H

#include <network_t.h>

void nnSetNetworkInput(nnNetwork *net, double *values, int valuec, int start_neuron);
int nnGetNetworkOutput(nnNetwork *net, double **values);
void nnCalcNetwork(nnNetwork *net);

#endif