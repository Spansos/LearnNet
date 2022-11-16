#include <learnnet.h>

LearnNet *CreateLearnNet(int mem_count, int *layersizes, int layerc, double (*activation_func)(double));
void CalcLearnNet(LearnNet *learnNet);
void FreeLearnNet(LearnNet *learnNet);
void SetLearnNetInput(LearnNet *learnNet, double *values, int valuec, int start_neuron);
int GetLearnNetOutput(LearnNet *learnNet, double **values);
int GetLearnNetSize(LearnNet *learnNet, int **layersizes);