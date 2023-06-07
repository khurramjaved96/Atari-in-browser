//
// Created by Khurram Javed on 2023-03-12.
//

#include "../../include/nn/weight_initializer.h"
#include <iostream>

WeightInitializer::WeightInitializer(float lower, float higher, int seed)
    : weight_sampler(lower, higher), mt(seed) {
  this->lower = lower;
  this->higher = higher;
}

Network *WeightInitializer::initialize_weights(Network *mygraph) {
  int counter = 0;
  for (auto &v : mygraph->list_of_all_vertices) {
    for (auto &incoming : v->incoming_edges) {
      incoming.weight = weight_sampler(this->mt);
    }
    counter++;
  }

  std::cout << "Setting outgoing weighs to 0\n";
  for (auto &v : mygraph->list_of_output_features) {
    for (auto &incoming : v->incoming_edges) {
      incoming.weight = 0;
    }
  }

  return mygraph;
}

void WeightInitializer::add_noise(Network *mygraph, bool output_only) {
  std::uniform_real_distribution<float> noise_sampler(-0.02, 0.02);
  if (output_only) {
    int counter = 0;
    for (auto &v : mygraph->list_of_output_features) {
      for (auto &incoming : v->incoming_edges) {
        incoming.weight = noise_sampler(this->mt);
      }
      counter++;
    }
  } else {
    int counter = 0;
    for (auto &v : mygraph->list_of_all_vertices) {
      for (auto &incoming : v->incoming_edges) {
        incoming.weight = noise_sampler(this->mt);
      }
      counter++;
    }
  }
}