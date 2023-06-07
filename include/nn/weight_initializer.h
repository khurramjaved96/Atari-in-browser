//
// Created by Khurram Javed on 2023-03-12.
//


#ifndef INCLUDE_NN_WEIGHT_INITIALIZER_H_
#define INCLUDE_NN_WEIGHT_INITIALIZER_H_
#include "networks/graph.h"

class WeightInitializer{
 protected:
  float lower;
  std::mt19937 mt;
  std::uniform_real_distribution<float> weight_sampler;
  float higher;
 public:
  WeightInitializer(float lower, float higher, int seed);
  Network * initialize_weights(Network * mygraph);
  void add_noise(Network * mygraph, bool output_only);
};

#endif //INCLUDE_NN_WEIGHT_INITIALIZER_H_
