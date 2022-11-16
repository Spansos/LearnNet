#ifndef LEARN_H
#define LEARN_H

#include <NeuralLib/network_t.h>

typedef struct learn_net_t {
    nnNetwork *neuralnet;
    double *memory;
} LearnNet;

LearnNet *CreateLearnNet(int mem_count, int *layersizes, int layerc, double (*activation_func)(double));
void CalcLearnNet(LearnNet *learnNet);
void FreeLearnNet(LearnNet *learnNet);
void SetLearnNetInput(LearnNet *learnNet, double *values, int valuec, int start_neuron);
int GetLearnNetOutput(LearnNet *learnNet, double **values);
int GetLearnNetSize(LearnNet *learnNet, int **layersizes);

#endif