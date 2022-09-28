#include <stdlib.h>
#include <math.h>
#include <neuralnet.h>


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


// sigmoid function, binds a value between 0 and 1, standard activation func
double sigmoid(double num) {
    double r_num = 1.0 / (1.0 + powf(M_E, num));
    return r_num;
}


// makes a new neuron, shouldn't be used outside of original source file
nnNeuron *nnCreateNeuron(nnLayer *prev_layer) {
    nnNeuron *neuron = calloc(1, sizeof(nnNeuron));
    neuron->weights = prev_layer ? calloc(prev_layer->neuronc, sizeof(double)) : NULL;
    neuron->weightc = prev_layer ? prev_layer->neuronc : 0;
    return neuron;
}


// makes a new layer, shouldn't be used outside of original source file
nnLayer *nnCreateLayer(int size, nnLayer *prev_layer) {
    nnLayer *layer = malloc(sizeof(nnLayer));
    nnNeuron **neurons = malloc(size * sizeof(nnNeuron*));
    for (int i=0; i<size; i++) {
        neurons[i] = nnCreateNeuron(prev_layer);
    }
    layer->neuronc = size;
    layer->neurons = neurons;
    return layer;
}


// makes a new network
nnNetwork *nnCreateNetwork(int *layersizes, int layerc, double (*activation_func)(double)) {
    if (activation_func==NULL) {
        activation_func = &sigmoid;
    }
    nnNetwork *net = malloc(sizeof(nnNetwork));
    nnLayer **layers = malloc(layerc * sizeof(nnLayer*));
    for (int i=0; i<layerc; i++) {
        layers[i] = nnCreateLayer(layersizes[i], (i>0) ? layers[i-1] : NULL);
    }
    net->activation_func = activation_func;
    net->layerc = layerc;
    net->layers = layers;
    return net;
}


// assign random values of [-1/2w, 1/2w] to the weights and [-1/2b, 1/2b] to the biases of a network
void nnInitRandom(nnNetwork *net, double w, double b) {
    for (int i=0; i < net->layerc; i++) {
        for (int j=0; j < net->layers[i]->neuronc; j++) {
            nnNeuron *neuron = net->layers[i]->neurons[j];
            neuron->bias = b * ((double)rand()/RAND_MAX - .5);
            for (int k=0; k < neuron->weightc; k++) {
                neuron->weights[k] = w * ((double)rand()/RAND_MAX - .5);
            }
        }
    }
}

// frees all memory of a network
void nnFreeNetwork(nnNetwork *net) {
    for (int i=0; i<net->layerc; i++) {
        for (int j=0; j<net->layers[i]->neuronc; j++) {
            free(net->layers[i]->neurons[j]->weights);
            free(net->layers[i]->neurons[j]);
        }
        free(net->layers[i]->neurons);
        free(net->layers[i]);
    }
    free(net->layers);
    free(net);
}


// sets network's first layer to values. length of values must be the same as first layer
// starts at start_neuron (first is 0).
void nnSetNetworkInput(nnNetwork *net, double *values, int valuec, int start_neuron) {
    for (int i=0; i < valuec; i++) {
        net->layers[0]->neurons[start_neuron + i]->out = values[i];
    }
}


// returns outs of last layer
int nnGetNetworkOutput(nnNetwork *net, double **values) {
    nnLayer *layer = net->layers[net->layerc-1];

    *values = malloc(layer->neuronc * sizeof(double));
    for (int i=0; i < layer->neuronc; i++) {
        (*values)[i] = layer->neurons[i]->out;
    }

    return layer->neuronc;
}


// returns the number of layers directly and the sizes of the layers by refrence
int nnGetNetworkSize(nnNetwork *net, int **layersizes) {
    *layersizes = malloc(net->layerc * sizeof(int));
    for (int i=0; i < net->layerc; i++) {
        (*layersizes)[i] = net->layers[i]->neuronc;
    }
    return net->layerc;
}


// calculates all neurons' out
void nnCalcNetwork(nnNetwork *net) {
    for (int i=1; i < net->layerc; i++) {
        nnLayer *prev_layer = net->layers[i-1];
        nnLayer *cur_layer = net->layers[i];

        for (int j=0; j < cur_layer->neuronc; j++) {
            nnNeuron *cur_neuron = cur_layer->neurons[j];

            cur_neuron->out = 0;
            for (int k=0; k < prev_layer->neuronc; k++) {
                cur_neuron->out += prev_layer->neurons[k]->out * cur_neuron->weights[k];
            }
            cur_neuron->out += cur_neuron->bias;
            cur_neuron->out = (*net->activation_func)(cur_neuron->out);
        }
    }
}


// makes a new network by adding two others
nnNetwork *nnAddNetworks(nnNetwork *net1, nnNetwork *net2) {
    // Check if the networks have the same shape, return NULL if not
    int *layersizes1;
    int *layersizes2;
    int layerc1 = nnGetNetworkSize(net1, &layersizes1);
    int layerc2 = nnGetNetworkSize(net2, &layersizes2);
    if (layerc1 != layerc2) {
        return NULL;
    }
    for (int i=0; i < layerc1; i++) {
        if (layersizes1[i] != layersizes2[i]) {
            return NULL;
        }
    }

    // network that will be returned
    nnNetwork *r_net = nnCreateNetwork(layersizes1, layerc1, NULL);

    // add all weigths and biases for all matching neurons
    for (int i=0; i<net1->layerc; i++) {
        nnLayer *cur_layer1 = net1->layers[i];
        nnLayer *cur_layer2 = net2->layers[i];
        for (int j=0; j<cur_layer1->neuronc; j++) {
            nnNeuron *neuron1 = cur_layer1->neurons[j];
            nnNeuron *neuron2 = cur_layer2->neurons[j];
            r_net->layers[i]->neurons[j]->bias = neuron1->bias + neuron2->bias;
            for (int k=0; k < neuron1->weightc; k++) {
                r_net->layers[i]->neurons[j]->weights[k] = neuron1->weights[k] + neuron2->weights[k];
            }
        }
    }
    return r_net;
}


// multiplies all biases and weights in a network
void nnMultNetwork(nnNetwork *net, double x) {
    for (int i=0; i < net->layerc; i++) {
        for (int j=0; j < net->layers[i]->neuronc; j++) {
            nnNeuron *neuron = net->layers[i]->neurons[j];

            neuron->bias = neuron->bias * x;
            for (int k; k < neuron->weightc; k++) {
                neuron->weights[k] = neuron->weights[k] * x;
            }
        }
    }
}