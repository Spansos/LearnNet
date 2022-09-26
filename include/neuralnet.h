#ifndef NNT_NEURALNET_H
#define NNT_NEURALNET_H

typedef struct nnt_neuron_t NntNeuron;
typedef struct nnt_layer_t NntLayer;

typedef struct nnt_network_t {
    int layerc;
    NntLayer **layers;
    double (*activation_func)(double);
} NntNetwork;

Network *NntCreateNetwork(int *layersizes, int layerc, double (*activation_func)(double));
void NntInitRandom(NntNetwork *net, double w, double b);
void NntCalcNetwork(NntNetwork *net);
void NntFreeNetwork(NntNetwork *net);
void NntSetNetworkInput(NntNetwork *net, double *values, int valuec, int start_neuron);
int NntGetNetworkOutput(NntNetwork *net, double **values);
int NntGetNetworkSize(NntNetwork *net, int **layersizes);
Network *NntAddNetworks(NntNetwork *net1, NntNetwork *net2);
void NntMultNetwork(NntNetwork *net, double x);

#endif