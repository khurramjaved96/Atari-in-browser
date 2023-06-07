//
// Created by Khurram Javed on 2023-03-12.
//


#ifndef INCLUDE_NN_WEIGHT_OPTIMIZER_H_
#define INCLUDE_NN_WEIGHT_OPTIMIZER_H_

#include <vector>
#include "networks/graph.h"
#include <string>


class Optimizer {
 public:
  virtual void update_weights(Network *mygraph, float error) = 0;
  virtual float get_average_gradient(Network *mygraph, std::string node_type) = 0;
};

class SGD : public Optimizer {
 protected:
  float step_size;
 public:
  SGD(float step_size);
  virtual void update_weights(Network *mygraph, float error) override;
  float get_average_gradient(Network *mygraph, std::string node_type) override;
};

class TIDBD : public Optimizer {
protected:
  float meta_step_size;
  std::vector<float> h;
  std::vector<float> step_sizes;
  std::vector<float> b2;
public:
  TIDBD(float step_size, float b2, float epsilon, Network *mygraph);
  virtual void update_weights(Network *mygraph, float error) override;
  float get_average_gradient(Network *mygraph, std::string node_type) override;
};

class Adam : public SGD {
 protected:
  float b1_value;
  float b2_value;
  std::vector<float> b1;
  std::vector<float> b2;
  float epsilon;
  float t;
  float b1_normalizer;
  float b2_normalizer;
 public:
  Adam(float step_size, float b1, float b2, float epsilon, Network *my_graph);
  void update_weights(Network *mygraph, float error) override;
  float get_average_gradient(Network *mygraph, std::string node_type) override;

};
#endif //INCLUDE_NN_WEIGHT_OPTIMIZER_H_
