//
// Created by Khurram Javed on 2023-03-18.
//

#include "../../include/nn/architure_initializer.h"
#include <iostream>
#include <random>

Network *ArchitectureInitializer::initialize_sprase_networks(
    Network *my_graph, int total_parameters, int parameters_per_feature,
    std::string vertex_type, float step_size) {

  int total_features = int(total_parameters / parameters_per_feature);
  for (int i = 0; i < total_features; i++) {
    my_graph->list_of_all_vertices.push_back(
        VertexFactory::get_vertex(vertex_type));
    auto sampler = std::uniform_int_distribution<int>(
        0, my_graph->list_of_all_vertices.size() - 2);
    for (int j = 0; j < parameters_per_feature; j++) {
      my_graph->add_edge(0, sampler(my_graph->mt),
                         my_graph->list_of_all_vertices.size() - 1, step_size);
    }
  }
  int total_vertices_features = my_graph->list_of_all_vertices.size();
  for (int i = 0; i < my_graph->list_of_output_features.size(); i++) {
    Vertex *output_vertex = my_graph->list_of_output_features[i];
    output_vertex->is_output = true;
    my_graph->list_of_all_vertices.push_back(output_vertex);
    for (int j = 0; j < total_vertices_features; j++) {
      my_graph->add_edge(0, j, my_graph->list_of_all_vertices.size() - 1,
                         step_size);
    }
  }
  return my_graph;
}