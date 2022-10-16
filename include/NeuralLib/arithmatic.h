#ifndef NN_ARITHMATIC_H
#define NN_ARITHMATIC_H

#include <network_t.h>

void nnInitRandom(nnNetwork *net, double w, double b);
int nnGetNetworkSize(nnNetwork *net, int **layersizes);
nnNetwork *nnAddNetworks(nnNetwork *net1, nnNetwork *net2);
void nnMultNetwork(nnNetwork *net, double x);


#endif