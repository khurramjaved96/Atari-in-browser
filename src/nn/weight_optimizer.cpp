//
// Created by Khurram Javed on 2023-03-12.
//

#include "../../include/nn/weight_optimizer.h"
#include <iostream>
#include <math.h>
#include <string>
SGD::SGD(float step_size) { this->step_size = step_size; }

void SGD::update_weights(Network *mygraph, float error) {
  int counter = 0;
  for (auto &v : mygraph->list_of_all_vertices) {
    //    float incoming_edges = v->incoming_edges.size();
    float step_size_scale = 1;
    if (this->step_size *
            (v->instantaneous_incoming_signal / v->trace_of_incoming_norm) >
        1) {
      step_size_scale =
          v->instantaneous_incoming_signal / v->trace_of_incoming_norm;
      std::cout << "Step_size scale = " << step_size_scale << std::endl;
      std::cout << "Instant norm " << v->instantaneous_incoming_signal
                << std::endl;
      std::cout << "Running norm " << v->trace_of_incoming_norm << std::endl;
    }
    for (auto &incoming : v->incoming_edges) {
      incoming.weight -=
          (this->step_size / (v->trace_of_incoming_norm * step_size_scale)) *
          incoming.gradient_trace * error;
    }
    counter++;
  }
}

TIDBD::TIDBD(float step_size, float b2, float epsilon, Network *mygraph) {
  this->meta_step_size = step_size;
  for (auto &v : mygraph->list_of_all_vertices) {
    for (auto &incoming : v->incoming_edges) {
      this->b2.push_back(1);
      this->h.push_back(0);
    }
  }
}

float TIDBD::get_average_gradient(Network *mygraph, std::string node_type) {
  return 0;
}

void TIDBD::update_weights(Network *mygraph, float error) {
  error *= -1;
  int counter = 0;
  for (auto &v : mygraph->list_of_all_vertices) {
    v->instantaneous_incoming_signal = 0;
    for (auto &e : v->incoming_edges) {
      v->instantaneous_incoming_signal +=
          e.step_size * e.gradient_trace * e.gradient_trace;
    }
    v->trace_of_incoming_norm *= 0.999;
    v->trace_of_incoming_norm += 0.001 * v->instantaneous_incoming_signal;
  }
  for (auto &v : mygraph->list_of_all_vertices) {

    if (v->trace_of_incoming_norm > 0.8) {
      for (auto &e : v->incoming_edges) {
        e.step_size /= v->trace_of_incoming_norm * 1.1;
      }
    }

    if (v->trace_of_incoming_norm < 1.0 &&
        v->instantaneous_incoming_signal < 1.0) {
      for (auto &e : v->incoming_edges) {
        //        std::cout << "Step size = " << e.step_size << std::endl;
        e.weight += (e.step_size * e.gradient_trace * error);
        this->h[counter] =
            this->h[counter] *
                (1 - e.step_size * e.gradient_trace * e.current_gradient) +
            e.step_size * error * e.gradient_trace;

        this->b2[counter] =
            this->b2[counter] * 0.999 + 0.001 * error * e.gradient_trace *
                                            this->h[counter] * error *
                                            e.gradient_trace * this->h[counter];
        e.step_size =
            exp(log(e.step_size) +
                (this->meta_step_size / (sqrt(this->b2[counter]) + 1e-8)) *
                    error * e.gradient_trace * this->h[counter]);
        if (e.step_size < 1e-8)
          e.step_size = 1e-8;
        if(e.step_size > 1e-1)
          e.step_size = 1e-1;
        counter++;
      }
    }
  }
//    }
}

Adam::Adam(float step_size, float b1, float b2, float epsilon, Network *mygraph)
    : SGD(step_size) {
  b1_value = b1;
  b2_value = b2;
  b1_normalizer = 1;
  b2_normalizer = 1;
  this->epsilon = epsilon;
  this->t = 0;
  for (auto &v : mygraph->list_of_all_vertices) {
    for (auto &incoming : v->incoming_edges) {
      this->b1.push_back(0);
      this->b2.push_back(0);
    }
  }
}

void Adam::update_weights(Network *mygraph, float error) {
  int counter = 0;
  b1_normalizer *= b1_value;
  b2_normalizer *= b2_value;
  this->t++;
  for (auto &v : mygraph->list_of_all_vertices) {
    float incoming_edges = v->incoming_edges.size();
    for (auto &incoming : v->incoming_edges) {
      this->b1[counter] = this->b1[counter] * b1_value +
                          (1 - b1_value) * incoming.gradient_trace * error;
      this->b2[counter] = this->b2[counter] * b2_value +
                          (1 - b2_value) * incoming.gradient_trace * error *
                              incoming.gradient_trace * error;
      float b1_grad = this->b1[counter] / (1 - b1_normalizer);
      float b2_grad = this->b2[counter] / (1 - b2_normalizer);
      incoming.weight -= (this->step_size / incoming_edges) * b1_grad /
                         (sqrt(b2_grad) + epsilon);
      if (incoming.weight > 2)
        incoming.weight = 2;
      if (incoming.weight < -2)
        incoming.weight = -2;
      counter++;
    }
  }
}

float SGD::get_average_gradient(Network *mygraph, std::string node_type) {
  float sum_of_gradients = 0;
  int counter = 0;
  for (int i = mygraph->list_of_all_vertices.size() - 1; i >= 0; i--) {
    if (mygraph->list_of_all_vertices[i]->type == node_type) {
      for (auto &e : mygraph->list_of_all_vertices[i]->incoming_edges) {
        sum_of_gradients += std::abs(e.gradient_trace);
        counter += 1;
      }
    }
  }
  return sum_of_gradients / counter;
}

float Adam::get_average_gradient(Network *mygraph, std::string node_type) {
  float sum_of_gradients = 0;
  int counter = 0;
  int counter_inner = 0;
  for (auto &v : mygraph->list_of_all_vertices) {
    for (auto &incoming : v->incoming_edges) {
      if (v->type == node_type) {
        float b1_grad = this->b1[counter] / (1 - b1_normalizer);
        float b2_grad = this->b2[counter] / (1 - b2_normalizer);
        sum_of_gradients += std::abs(b1_grad / (sqrt(b2_grad) + epsilon));
        counter_inner++;
      }
      counter += 1;
    }
  }
  return sum_of_gradients / counter_inner;
}
