#ifndef LEARN_H
#define LEARN_H

#include <NeuralLib/network_t.h>

typedef struct learn_net_t {
    nnNetwork *neuralnet;
    int memoryc;
} LearnNet;

LearnNet *CreateLearnNet(int memoryc, int *layersizes, int layerc, double (*activation_func)(double));
void CalcLearnNet(LearnNet *learnNet);
void FreeLearnNet(LearnNet *learnNet);
void SetLearnNetInput(LearnNet *learnNet, double *values, int valuec, int start_neuron);
void ResetNetMemory(LearnNet *learnnet);
int GetLearnNetOutput(LearnNet *learnNet, double **values);
int GetLearnNetSize(LearnNet *learnNet, int **layersizes);

#endif