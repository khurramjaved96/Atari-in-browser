//
// Created by Khurram Javed on 2021-03-16.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "../include/utils.h"

std::vector<float> &update_statistics_and_normalize(
    std::vector<unsigned char> &features,
    std::vector<float> &normalized_features_container,
    std::vector<float> &features_mean, std::vector<float> &features_variance,
    float min_variance,
    float decay_rate, bool update_statistics) {
  if (normalized_features_container.size() == 0) {
    std::cout << "Adding space\n";
    for (auto s : features) {
      normalized_features_container.push_back(float(s));
      features_mean.push_back(0);
      features_variance.push_back(1);
    }
  } else {
    for (int counter = 0; counter < features.size(); counter++) {
      float old_mean = features_mean[counter];
      float f_divided = float(features[counter]) / 255;
      if(update_statistics) {
        features_mean[counter] =
            features_mean[counter] * decay_rate + (1 - decay_rate) * f_divided;
        features_variance[counter] = features_variance[counter] * decay_rate +
                                     (1 - decay_rate) *
                                         (f_divided - features_mean[counter]) *
                                         (f_divided - old_mean);
      }
      if (features_variance[counter] < min_variance)
        features_variance[counter] = min_variance;
      normalized_features_container[counter] =
          (features_mean[counter] - f_divided) / features_variance[counter];
    }
  }
  normalized_features_container[0] = 1;
  return normalized_features_container;
}
//
//std::vector<float> &update_feature_correlation(
//    std::vector<float> &normalized_features_container,
//    std::vector<float> &feature_error_correlation, float &td_error,
//    float &mean_td_error,
//    float &variance_td_error, bool update_statistics) {
//  if (normalized_features_container.size() == 0) {
//    std::cout << "Adding space\n";
//    for (auto s : features) {
//      normalized_features_container.push_back(float(s));
//      features_mean.push_back(0);
//      features_variance.push_back(1);
//    }
//  } else {
//    for (int counter = 0; counter < features.size(); counter++) {
//      float old_mean = features_mean[counter];
//      float f_divided = float(features[counter]) / 255;
//      if(update_statistics) {
//        features_mean[counter] =
//            features_mean[counter] * decay_rate + (1 - decay_rate) * f_divided;
//        features_variance[counter] = features_variance[counter] * decay_rate +
//                                     (1 - decay_rate) *
//                                         (f_divided - features_mean[counter]) *
//                                         (f_divided - old_mean);
//      }
//      if (features_variance[counter] < min_variance)
//        features_variance[counter] = min_variance;
//      normalized_features_container[counter] =
//          (features_mean[counter] - f_divided) / features_variance[counter];
//    }
//  }
//  return normalized_features_container;
//}
//

void print_vector(std::vector < float > const &v) {
  std::cout << "[";
  int counter = 0;
  for (auto i = v.begin(); i != v.end(); ++i){
    std::cout << " " << std::setw(3) << *i << ",";
    if (counter % 50 == 49) std::cout << "\n";
    counter++;
  }
  std::cout << "]\n";
}

void print_vector(std::vector < char > const &v) {
std::cout << "[";
int counter = 0;
for (auto i = v.begin(); i != v.end(); ++i){
std::cout << " " << std::setw(3) << *i << ",";
if (counter % 50 == 49) std::cout << "\n";
counter++;
}
std::cout << "]\n";
}

void print_vector(std::vector < int > const &v) {
  std::cout << "[";
  int counter = 0; for (auto i = v.begin(); i != v.end(); ++i) {
    std::cout << " " << std::setw(3) << *i << ",";
    if (counter % 100 == 99) std::cout << "\n";
    counter++;
  }
  std::cout << "]\n";
}

void print_vector(std::vector < long unsigned int > const &v) {
std::cout << "[";
int counter = 0; for (auto i = v.begin(); i != v.end(); ++i) {
std::cout << " " << std::setw(3) << *i << ",";
if (counter % 100 == 99) std::cout << "\n";
counter++;
}
std::cout << "]\n";
}

void print_matrix(std::vector < std::vector < int >> const &v) {
  int counter = 0;
  for (
  int temp = 0; temp<v.size(); temp++) {
    if (temp > 50) {
      std::cout << "Truncating output\n";
      break;
    }
    std::cout << "[";
    int counter_inner = 0;
    for ( auto i = v[temp].begin(); i != v[temp].end(); ++i) {
      if (counter_inner > 50) {
        std::cout << ", ... , ]";
        break;
      }
      std::cout << " " << std::setw(3) << *i << ",";
      //            if (counter % 50 == 49) std::cout << "\n";
      counter++;
      counter_inner++;
      }
    std::cout << "]\n";
    }
  std::cout << "\n";
}
//
//NetworkVisualizer::NetworkVisualizer(std::vector<Neuron *> all_neurons) {
//  this->all_neurons = all_neurons;
//}
//
//void NetworkVisualizer::generate_dot(int time_step) {
//  dot_string = "digraph network{\n"
//               "\tnode [shape = circle];\n";
//
//  for (auto &it : all_neurons) {
//    for (auto &os : it->outgoing_synapses) {
//      if (os->input_neuron->is_mature && os->output_neuron->is_mature) {
//        auto current_n = os;
//        dot_string += "\t" + std::to_string(current_n->input_neuron->id)
//            + "->" + std::to_string(current_n->output_neuron->id)  //+ ";\n";
//            + "[label = \"" + std::to_string(os->weight) + "\"];\n";
//      }
//    }
//  }
//  dot_string += "\n}";
//  std::ofstream dot_file("vis/" + std::to_string(time_step) + "_simple" + ".gv");
//  dot_file << dot_string;
//  dot_file.close();
//}
//
//std::string NetworkVisualizer::get_graph(int time_step) {
//  dot_string = "digraph network{\n"
//               "\tnode [shape = circle];\n";
//
////  Input neuron cluster
//  dot_string += "subgraph cluster_0 {\n"
//                "\t    style=filled;\n"
//                "\t\tcolor=lightgrey;\n"
//                "\t\tnode [style=filled,color=white];\n"
//                "\t\t";
//
//  for (auto &it : all_neurons) {
//    if (it->is_input_neuron) {
//      dot_string += std::to_string(it->id) + " [color = lightgreen];\n";
//    }
//  }
//
//  dot_string += "\n\tlabel = \"Input features\";\n"
//                "\t}";
//
//  dot_string += "subgraph cluster_1 {\n"
//                "\t    style=filled;\n"
//                "\t\tcolor=lightgrey;\n"
//                "\t\tnode [style=filled,color=white];\n"
//                "\t\t";
//
//  for (auto &it : all_neurons) {
//    if (it->is_output_neuron) {
//      dot_string += std::to_string(it->id) + " [color = lightblue];\n";
//    }
//  }
//
//  dot_string += "\n\tlabel = \"Output predictions\";\n"
//                "\t}";
//
//  dot_string += "subgraph cluster_2 {\n"
//                "\t    style=filled;\n"
//                "\t\tcolor=lightgrey;\n"
//                "\t\tnode [style=filled,color=white];\n"
//                "\t\t";
//
//  int total_fringe_neurons = 0;
//  int total_incoming_fringe_synapses = 0;
//  for (auto &it : all_neurons) {
//    if (!it->is_mature) {
//      total_fringe_neurons += 1;
//      total_incoming_fringe_synapses += it->incoming_synapses.size();
//
//    }
//  }
//
//  dot_string += std::to_string(-1) + "  [color = yellow];\n";
//  dot_string += std::to_string(-1) + " [label = " + "\"Features:" + std::to_string(total_fringe_neurons) + " Synapses:"
//      + std::to_string(total_incoming_fringe_synapses) + "\"];\n";
//
//  dot_string += "\n\tlabel = \"Fringe\";\n"
//                "\t}";
//
//  dot_string += "subgraph cluster_3 {\n"
//                "\t    style=filled;\n"
//                "\t\tcolor=lightgrey;\n"
//                "\t\tnode [style=filled,color=white];\n"
//                "\t\t";
//
//  for (auto &it : all_neurons) {
//    if (it->is_mature && !it->is_input_neuron && !it->is_output_neuron && it->outgoing_synapses.size() > 0
//        && it->incoming_synapses.size() > 0 && it->neuron_utility >= it->outgoing_synapses[0]->utility_to_keep) {
//      dot_string += std::to_string(it->id) + " [color = pink];\n";
//
//    }
//  }
//
//  dot_string += "\n\tlabel = \"Backbone\";\n"
//                "\t}";
//
//  for (auto &it : all_neurons) {
//    for (auto &os: it->outgoing_synapses) {
//      if (os->input_neuron->is_mature && os->output_neuron->is_mature
//          && os->synapse_utility >= os->utility_to_keep) {
//        auto current_n = os;
//        dot_string += "\t" + std::to_string(current_n->input_neuron->id)
//            + "->" + std::to_string(current_n->output_neuron->id) //+ ";\n";
//            + "[label = \"" + std::to_string(os->weight) + ":" + std::to_string(os->synapse_utility_to_distribute)
//            + "\"];\n";
//      } else {
////              if( os->synapse_utility < os->utility_to_keep && false){
//////                auto current_n = os;
//////                dot_string += "\t" + std::to_string(current_n->input_neuron->id)
//////                    + "->" + std::to_string(current_n->output_neuron->id) //+ ";\n";
//////                    + "[label = \"" + std::to_string(os->weight) + ":" + std::to_string(os->synapse_utility_to_distribute) + "\"];\n";
//////
//////                dot_string += "\t" + std::to_string(current_n->input_neuron->id)
//////                    + "->" + std::to_string(current_n->output_neuron->id) //+ ";\n";
//////                    + "[color = darkred];\n";
////              }
////              else
//      }
//
//    }
//  }
//  dot_string += "\n}";
//  return dot_string;
//}
//
//std::string NetworkVisualizer::get_graph_detailed(int time_step) {
//  dot_string = "digraph network{\n"
//               "\tconcentrate=True;\n"
//               "\trankdir=TB;\n"
//               "\tnode [shape=record];";
//
//  for (auto &it : all_neurons) {
//    dot_string += "\t" + std::to_string(it->id)
//        + " [label=\"" + std::to_string(it->id) + "\\n|"
//        + "{past_act_size:|err_grad_size:|gradient_activation:}"
//        + "|{" + std::to_string(it->past_activations.size()) + "|"
//        + std::to_string(it->error_gradient.size()) + "|"
//        + std::to_string(it->value) + "}"
//        + "\"];\n";
//    for (auto &os : it->outgoing_synapses) {
//      auto current_n = os;
//      dot_string += "\t" + std::to_string(current_n->input_neuron->id)
//          + "->" + std::to_string(current_n->output_neuron->id) + ";\n";
//      //+ " [label = \"gq_size:" + std::to_string(os->grad_queue.size())
//      //+ " credit:" + std::to_string(os->credit) + "\"];\n";
//
//      // + "[label = \"" + std::to_string(os->weight) + "\"];\n";
//    }
//  }
//  dot_string += "\n}";
//  return dot_string;
//}
//
//void NetworkVisualizer::generate_dot_detailed(int time_step) {
//  dot_string = "digraph network{\n"
//               "\tconcentrate=True;\n"
//               "\trankdir=TB;\n"
//               "\tnode [shape=record];";
//
//  for (auto &it : all_neurons) {
//    dot_string += "\t" + std::to_string(it->id)
//        + " [label=\"" + std::to_string(it->id) + "\\n|"
//        + "{past_act_size:|err_grad_size:|gradient_activation:}"
//        + "|{" + std::to_string(it->past_activations.size()) + "|"
//        + std::to_string(it->error_gradient.size()) + "|"
//        + std::to_string(it->value) + "}"
//        + "\"];\n";
//    for (auto &os : it->outgoing_synapses) {
//      auto current_n = os;
//      dot_string += "\t" + std::to_string(current_n->input_neuron->id)
//          + "->" + std::to_string(current_n->output_neuron->id) + ";\n";
//      //+ " [label = \"gq_size:" + std::to_string(os->grad_queue.size())
//      //+ " credit:" + std::to_string(os->credit) + "\"];\n";
//
//      // + "[label = \"" + std::to_string(os->weight) + "\"];\n";
//    }
//  }
//  dot_string += "\n}";
//  std::ofstream dot_file("vis/" + std::to_string(time_step) + "_detailed" + ".gv");
//  dot_file << dot_string;
//  dot_file.close();
//}
