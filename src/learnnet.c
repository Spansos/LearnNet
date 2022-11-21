#include <learnnet.h>
#include <stdlib.h>
#include <NeuralLib/calc.h>

LearnNet *CreateLearnNet(int memoryc, int *layersizes, int layerc, double (*activation_func)(double)) {
    LearnNet *lnet = malloc(sizeof(LearnNet));
    
    layersizes[0] += memoryc;
    layersizes[layerc-1] += memoryc;

    nnNetwork *net = nnCreateNetwork(layersizes, layerc, activation_func);
    
    lnet->memoryc = memoryc;
    lnet->neuralnet = net;

    return lnet;
}

void FreeLearnNet(LearnNet *learnNet) {
    nnFreeNetwork(learnNet->neuralnet);
    free(learnNet);
}

void CalcLearnNet(LearnNet *learnNet) {
    nnLayer *inlayer = learnNet->neuralnet->layers[0];
    nnLayer *outlayer = learnNet->neuralnet->layers[learnNet->neuralnet->layerc-1];
    for (int i=0; i < learnNet->memoryc; i++) {
        inlayer->neurons[i]->out = outlayer->neurons[i]->out;
    }
    nnCalcNetwork(learnNet->neuralnet);
}

void SetLearnNetInput(LearnNet *learnNet, double *values, int valuec, int start_neuron) {
    nnSetNetworkInput(learnNet->neuralnet, values, valuec, learnNet->memoryc+start_neuron);
}

int GetLearnNetOutput(LearnNet *learnNet, double **values) {
    nnLayer *layer = learnNet->neuralnet->layers[learnNet->neuralnet->layerc-1];

    int valneurc = layer->neuronc - learnNet->memoryc;

    *values = malloc(valneurc * sizeof(double));
    for (int i=0; i < valneurc; i++) {
        (*values)[i] = layer->neurons[learnNet->memoryc + i]->out;
    }

    return valneurc;
}

void FreeNetMemory(LearnNet *learnnet) {
    nnLayer *inlayer = learnnet->neuralnet->layers[0];
    nnLayer *outlayer = learnnet->neuralnet->layers[learnnet->neuralnet->layerc-1];
    for (int i=0; i < learnnet->memoryc; i++) {
        inlayer->neurons[i]->out = 0;
        outlayer->neurons[i]->out = 0;
    }
}