//
// Created by Khurram Javed on 2023-04-26.
//

#ifndef _VARIABLES_H_
#define _VARIABLES_H_

long long int ui_steps = 0;
int update_intervel = 1;
static int seed = 0;
std::vector<std::string> my_models;
static int game_id = 0;
bool show_normalized_features = false;
bool show_weights = false;
bool show_step_sizes = false;
float lambda = 0.0;
float discount_factor = 0.975;

int fps = 1;
unsigned int environment_steps = 0;
bool frozen_agent = false;
bool no_render = false;
bool to_step = false;
// float critic_step_size = 3e-4;
// float meta_step_size = 3e-3;
float critic_step_size = 1e-8;
float meta_step_size = 1e-3;

ArchitectureInitializer network_initializer = ArchitectureInitializer();
Network *critic = new StateValueNetwork(100800, 0);
StateValueNetwork *critic_v = static_cast<StateValueNetwork *>(critic);

Optimizer *opti_critic;
WeightInitializer weight_initializer(-0.02, 0.02, 0);
float S_value = critic_v->get_prediction();

std::vector<unsigned char> features;
std::vector<float> normalized_features;
std::vector<float> features_mean;
std::vector<float> features_variance;

float mean_td_error;
float variance_td_error;
std::vector<float> feature_error_correlation;
float fps_estimate = 0;

float reward = 0;
#endif //_VARIABLES_H_
