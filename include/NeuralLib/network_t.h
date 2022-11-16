#ifndef NN_NETWORK_T_H
#define NN_NETWORK_T_H

typedef struct nn_neuron_t {
    double out;
    double bias;
    double *weights;
    int weightc;
} nnNeuron;

typedef struct nn_layer_t {
    int neuronc;
    nnNeuron **neurons;
} nnLayer;

typedef struct nn_network_t {
    int layerc;
    nnLayer **layers;
    double (*activation_func)(double);
} nnNetwork;

nnNetwork *nnCreateNetwork(int *layersizes, int layerc, double (*activation_func)(double));
void nnFreeNetwork(nnNetwork *net);


#endif