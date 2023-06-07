//
// Created by Khurram Javed on 2023-03-12.
//

#ifndef INCLUDE_NN_NETWORKS_GRAPH_H_
#define INCLUDE_NN_NETWORKS_GRAPH_H_
#include "vertex.h"
#include <random>
#include <string>
#include <vector>

class Vertex;

class Edge {
public:
  int edge_id;
  static int edge_id_generator;
  int from;
  int to;
  float gradient_trace;
  float utility;
  float current_gradient;
  float h = 0;
  float local_utility;
  float step_size;
  float weight;
  Edge(float weight, int from, int to, float step_size);
};

class Network {
protected:
  int input_vertices;

public:
  std::mt19937 mt;
  void load_model_from_file(std::string file_path);
  std::vector<Vertex *> list_of_all_vertices;
  std::vector<Vertex *> list_of_output_features;
  Network();
  Network(int input_vertices, int seed);
  void add_edge(float weight, int from, int to, float step_size);
  void print_graph();
  void estimate_gradient();
  void set_input_values(std::vector<float> inp);
  float update_values();
  void decay_weight_grad(float decay_rate);
  void zero_vertex_grad();
  void set_gradients_for_output_vertices(std::vector<float> gradients);
  std::string serialize_graph();
};
class PolicyNetwork : public Network {
public:
  int last_action;
  PolicyNetwork(int input_vertices, int seed, int actions);
  int sample_action();
  int sample_action(std::vector<int> legal_actions);
  int sample_action_with_noise(std::vector<int> legal_actions, float noise);
  void set_gradients_for_output(int action);
  float softmax_noralization_term;
  std::vector<float> prediction_logits;
};



class StateValueNetwork : public Network {
public:
  StateValueNetwork(int input_vertices, int seed);
  void set_gradients_for_output();
  float get_prediction() const;
  float prediction;
};

#endif // INCLUDE_NN_NETWORKS_GRAPH_H_
