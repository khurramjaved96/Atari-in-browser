//
// Created by Khurram Javed on 2023-03-12.
//

#include "../../../include/nn/networks/graph.h"
#include "../../../include/nn/networks/vertex.h"
#include "../../../include/utils.h"
#include <fstream>
#include <iostream>
#include <math.h>
#include <random>
#include <string>
#include <vector>
// Graph implementation

void Network::add_edge(float weight, int from, int to, float step_size) {
  Edge my_edge = Edge(weight, from, to, step_size);
  list_of_all_vertices[to]->incoming_edges.push_back(my_edge);
  list_of_all_vertices[from]->sum_of_outgoing_weights += std::abs(weight);
}

void Network::print_graph() {
  int counter = 0;
  std::cout << "digraph g {\n";
  for (auto v : this->list_of_all_vertices) {
    std::cout << v->id << " [label=\"" << std::to_string(counter) << "\"];"
              << std::endl;
    for (auto incoming : v->incoming_edges) {
      std::cout << incoming.from << " -> " << incoming.to << "[label=\""
                << std::to_string(incoming.weight) << "\"];" << std::endl;
    }
    counter++;
  }
  std::cout << "}\n";
}

std::string Network::serialize_graph() {
  std::string empty_string;
  int counter = 0;
  for (auto v : this->list_of_all_vertices) {
    for (auto incoming : v->incoming_edges) {
      empty_string += std::to_string(incoming.from) + " " +
                      std::to_string(incoming.to) + " " +
                      std::to_string(incoming.weight) + " " +
                      std::to_string(incoming.step_size) + " " + v->type;
      if (v->is_output) {
        empty_string += " output\n";
      } else {
        empty_string += " inner\n";
      }
    }
    counter++;
  }
  return empty_string;
}

void Network::load_model_from_file(std::string file_path) {
  this->list_of_all_vertices.clear();
  this->list_of_output_features.clear();
  for (int i = 0; i < this->input_vertices; i++) {
    auto v = VertexFactory::get_vertex("linear");
    this->list_of_all_vertices.push_back(v);
  }
  int a, b;
  float c;
  float step_size;
  std::string type;
  std::string output_type;
  std::ifstream infile(file_path);
  int old_b = -1;
  std::cout << "File path " << file_path << std::endl;
  while (infile >> a >> b >> c >> step_size >> type >> output_type) {

    if (b != old_b) {
      auto v = VertexFactory::get_vertex(type);
      this->list_of_all_vertices.push_back(v);
      if (output_type == "output") {
        this->list_of_output_features.push_back(v);
        v->is_output = true;
      }
    }
    old_b = b;
  }
  std::ifstream infile2(file_path);
  while (infile2 >> a >> b >> c >> step_size >> type >> output_type) {
    this->add_edge(c, a, b, step_size);
  }
}

Network::Network(int input_vertices, int seed) : mt(seed) {

  this->input_vertices = input_vertices;
  Vertex *mem = new Vertex[input_vertices];
  for (int i = 0; i < input_vertices; i++) {
    this->list_of_all_vertices.push_back(&mem[i]);
  }
}

PolicyNetwork::PolicyNetwork(int input_vertices, int seed, int actions)
    : Network(input_vertices, seed) {
  int output_vertices = actions;
  Vertex *mem = new Vertex[output_vertices];
  for (int i = 0; i < output_vertices; i++) {
    this->prediction_logits.push_back(0);
    this->list_of_output_features.push_back(&mem[i]);
  }
}

StateValueNetwork::StateValueNetwork(int input_vertices, int seed)
    : Network(input_vertices, seed) {
  this->prediction = 0;
  this->list_of_output_features.push_back(new Vertex());
}

float StateValueNetwork::get_prediction() const {
  return this->list_of_output_features[0]->value;
}

int PolicyNetwork::sample_action() {
  this->softmax_noralization_term = 0;
  float max = -1000000;
  for (int i = 0; i < list_of_output_features.size(); i++) {
    float val = list_of_output_features[i]->forward();
    if (val > max) {
      max = val;
    }
  }
  for (int i = 0; i < list_of_output_features.size(); i++) {
    float val = exp(list_of_output_features[i]->forward() - max);
    this->prediction_logits[i] = val;
    this->softmax_noralization_term += val;
  }
  std::discrete_distribution<int> d(prediction_logits.begin(),
                                    prediction_logits.end());
  this->last_action = d(this->mt);
  return this->last_action;
}

int PolicyNetwork::sample_action(std::vector<int> legal_actions) {
  this->softmax_noralization_term = 0.00001;
  float max = -1000000;
  for (int i = 0; i < list_of_output_features.size(); i++) {
    float val = list_of_output_features[i]->forward();
    if (val > max) {
      max = val;
    }
  }

  for (int i = 0; i < list_of_output_features.size(); i++) {
    bool legal = false;
    for (int j = 0; j < legal_actions.size(); j++) {
      if (legal_actions[j] == i) {
        legal = true;
        break;
      }
    }
    float val;
    if (legal)
      val = exp(list_of_output_features[i]->forward() - max);
    else
      val = 0;
    this->prediction_logits[i] = val;
    this->softmax_noralization_term += val;
  }
  std::discrete_distribution<int> d(prediction_logits.begin(),
                                    prediction_logits.end());
  this->last_action = d(this->mt);
  bool legal = false;
  for (int j = 0; j < legal_actions.size(); j++) {
    if (legal_actions[j] == this->last_action) {
      legal = true;
      break;
    }
  }
  if (!legal) {
    if (legal_actions.size() > 0) {
      return legal_actions[0];
    } else {
      return 0;
    }
  }
  return this->last_action;
}

int PolicyNetwork::sample_action_with_noise(std::vector<int> legal_actions,
                                            float noise) {

  if (legal_actions.size() == 0) {
    return 0;
  }
  this->softmax_noralization_term = 0.00001;
  float max = -1000000;
  for (int i = 0; i < list_of_output_features.size(); i++) {
    float val = list_of_output_features[i]->forward();
    if (val > max) {
      max = val;
    }
  }
  float sum_of_legal_logits = 0;
  std::vector<float> legal_action_logits;
  for (int i = 0; i < list_of_output_features.size(); i++) {
    bool legal = false;
    for (int j = 0; j < legal_actions.size(); j++) {
      if (legal_actions[j] == i) {
        legal = true;
        break;
      }
    }
    float val;
    if (legal) {
      val = exp(list_of_output_features[i]->forward() - max);
      sum_of_legal_logits += val;
      legal_action_logits.push_back(val);
    } else
      val = 0;
    for (int i = 0; i < legal_action_logits.size(); i++) {
      //      This assures that every action has a 1% prob
      legal_action_logits[i] += sum_of_legal_logits * noise;
    }
    if (legal)
      this->prediction_logits[i] = val * noise;
    else
      this->prediction_logits[i] = val;
    this->softmax_noralization_term += val;
  }
  std::discrete_distribution<int> d(legal_action_logits.begin(),
                                    legal_action_logits.end());
  int sample = d(this->mt);
  this->last_action = legal_actions[sample];
  return this->last_action;
}

void PolicyNetwork::set_gradients_for_output(int action) {
  for (int i = 0; i < list_of_all_vertices.size(); i++)
    list_of_all_vertices[i]->d_out_d_vertex = 0;
  float prop = prediction_logits[action] / this->softmax_noralization_term;
  for (int i = 0; i < list_of_output_features.size(); i++) {
    if (i == action)
      list_of_output_features[i]->d_out_d_vertex =
          (1 - prediction_logits[i] / this->softmax_noralization_term);
    else
      list_of_output_features[i]->d_out_d_vertex =
          (-prediction_logits[i] / this->softmax_noralization_term);
  }
}

void Network::set_input_values(std::vector<float> inp) {
  for (int i = 0; i < input_vertices; i++) {
    this->list_of_all_vertices[i]->value = inp[i];
  }
}

float Network::update_values() {
  for (int i = input_vertices; i < list_of_all_vertices.size(); i++) {
    list_of_all_vertices[i]->value = 0;
    //    This term prevents overshoot when learning with a large step-size
    for (auto &e : list_of_all_vertices[i]->incoming_edges) {
      list_of_all_vertices[i]->value +=
          list_of_all_vertices[e.from]->forward() * e.weight;
      e.local_utility =
          e.local_utility * 0.999 +
          0.001 * std::abs(list_of_all_vertices[e.from]->forward() * e.weight);
    }
  }
  return 0;
}

void Network::set_gradients_for_output_vertices(std::vector<float> gradients) {
  for (int i = 0; i < list_of_output_features.size(); i++) {
    list_of_output_features[i]->d_out_d_vertex = gradients[i];
  }
}

void StateValueNetwork::set_gradients_for_output() {
  for (int i = 0; i < list_of_all_vertices.size(); i++)
    list_of_all_vertices[i]->d_out_d_vertex = 0;
  list_of_output_features[0]->d_out_d_vertex = 1;
}

void Network::zero_vertex_grad() {
  for (int i = 0; i < list_of_all_vertices.size(); i++) {
    this->list_of_all_vertices[i]->d_out_d_vertex = 0;
  }
}

void Network::decay_weight_grad(float decay_rate) {
  for (int i = list_of_all_vertices.size() - 1; i >= 0; i--) {
    for (auto &e : this->list_of_all_vertices[i]->incoming_edges) {
      e.gradient_trace *= decay_rate;
    }
  }
}

void Network::estimate_gradient() {

  //  Back-prop implementation
  for (int i = list_of_all_vertices.size() - 1; i >= 0; i--) {
    this->list_of_all_vertices[i]->d_out_d_vertex =
        this->list_of_all_vertices[i]->backward(
            this->list_of_all_vertices[i]->value) *
        this->list_of_all_vertices[i]->d_out_d_vertex;

    for (auto &e : this->list_of_all_vertices[i]->incoming_edges) {
      float cur_gradient = this->list_of_all_vertices[e.from]->forward() *
                           this->list_of_all_vertices[e.to]->d_out_d_vertex;
      e.gradient_trace += cur_gradient;
      e.current_gradient = cur_gradient;
      this->list_of_all_vertices[e.from]->d_out_d_vertex +=
          this->list_of_all_vertices[e.to]->d_out_d_vertex * e.weight;
    }
  }
}

Network::Network() {}
