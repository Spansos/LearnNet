#ifndef NT_NEURALNET_H
#define NT_NEURALNET_H

typedef struct nn_neuron_t nnNeuron;
typedef struct nn_layer_t nnLayer;

typedef struct nn_network_t {
    int layerc;
    nnLayer **layers;
    double (*activation_func)(double);
} nnNetwork;

nnNetwork *nnCreateNetwork(int *layersizes, int layerc, double (*activation_func)(double));
void nnInitRandom(nnNetwork *net, double w, double b);
void nnCalcNetwork(nnNetwork *net);
void nnFreeNetwork(nnNetwork *net);
void nnSetNetworkInput(nnNetwork *net, double *values, int valuec, int start_neuron);
int nnGetNetworkOutput(nnNetwork *net, double **values);
int nnGetNetworkSize(nnNetwork *net, int **layersizes);
nnNetwork *nnAddNetworks(nnNetwork *net1, nnNetwork *net2);
void nnMultNetwork(nnNetwork *net, double x);

#endif