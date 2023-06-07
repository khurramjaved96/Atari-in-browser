#include "src/ale_interface.hpp"
#include <iostream>

#include "gui_initialization.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "include/nn/architure_initializer.h"
#include "include/nn/networks/graph.h"
#include "include/nn/weight_initializer.h"
#include "include/nn/weight_optimizer.h"
#include "include/utils.h"
#include "variables.h"
#include <chrono>
#include <ctime>
#include <fstream>

ale::ALEInterface my_env;

std::ifstream actions_list("../SpaceInvadersNoFrameskip-v4.txt", std::ios_base::binary);

std::ifstream reward_list("../rewards_SpaceInvadersNoFrameskip-v4.txt", std::ios_base::binary);

int total_reward = 0;
int total_steps = 0;
void agent_step() {
  //  Agent loop
  char x;
  actions_list >> x;
  int total = 0;

  environment_steps++;
  ale::ActionVect legal_actions = my_env.getMinimalActionSet();
  if(int(x) == 82) {

    my_env.reset_game();
    std::cout << "Resetting episode\n";
    std::cout << "Ep length " << total_steps << " Ep reward " << total_reward << std::endl;
    total_steps = 0;
    total_reward = 0;
  }
  else {
    reward = my_env.act(legal_actions[int(x) - 97]);
    char y = 'a';
    int flag = true;
    while(y!=','){
//      std::cout << y << " " << int(y) << std::endl;
      reward_list >> y;
      if(y == '.')
        flag = false;
      if(flag)
        total = total*10 + (int(y) - 48);
    }
    if(reward != 0 || total != 0){
      std::cout << "Exp reward " << reward << "\n";
      std::cout <<  "GT reward " << total << "\n";
    }

    total_reward+= reward;
    total_steps++;
//    std::cout << legal_actions[int(x) - 97] << ", ";
  }
//  for(int i = 0; i<legal_actions.size(); i++)
//    std::cout << legal_actions[i] << ", ";
//  std::cout << std::endl;
//  std::cout << "Action = " << legal_actions[int(x) - 97] << std::endl;
//  std::cout << "action = " << int(x) - 97 << std::endl;
  //  We implement code for training the critic
//  critic_v->decay_weight_grad(lambda * discount_factor);
//  critic_v->set_gradients_for_output();
//  critic_v->estimate_gradient();
  ale::Action a = legal_actions[rand() % legal_actions.size()];
//  if(reward == 1.5)
//    my_env.act(a);
//  else
//    reward = my_env.act(a);
  my_env.getScreenRGB(features);
//  update_statistics_and_normalize(features, normalized_features, features_mean,
//                                   features_variance, 0.1, 0.995, environment_steps < 10000);

//  critic_v->set_input_values(normalized_features);
//  critic_v->update_values();
//  float S_prime_val = critic_v->get_prediction();
//  float delta = reward + discount_factor * S_prime_val - S_value;
//  if (!frozen_agent)
//    opti_critic->update_weights(critic, -delta);
//  S_value = S_prime_val;
//  reward = 0;
}

void add_imgui_interface(GLuint &textureID) {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  if (!no_render) {
    std::vector<unsigned char> norm_features;

    unsigned char *data;
    if (show_normalized_features) {
      for (int i = 0; i < features.size(); i++) {
        float mean_sub_feature = float(features[i]) / 255 - features_mean[i];
        mean_sub_feature /= sqrt(features_variance[i]);

        if (mean_sub_feature < 0)
          mean_sub_feature *= -1;
        norm_features.push_back(uint8_t(mean_sub_feature * 255));
      }
      data = reinterpret_cast<unsigned char *>(norm_features.data());
    } else if (show_weights) {
      std::vector<float> temp;
      float max = -1000;
      float min = 1000;
      for (int i = 0;
           i < critic_v->list_of_output_features[0]->incoming_edges.size();
           i++) {
        float mean_sub_feature =
            std::abs(critic_v->list_of_output_features[0]
                             ->incoming_edges[i]
                             .weight);
        if (mean_sub_feature > max) {
          max = mean_sub_feature;
        }
        if (mean_sub_feature < min)
          min = mean_sub_feature;

        temp.push_back(mean_sub_feature);
      }
      for (int i = 0; i < temp.size(); i++) {
        norm_features.push_back(uint8_t(255 * ((temp[i] - min) / (max - min))));
      }
      data = reinterpret_cast<unsigned char *>(norm_features.data());
    } else if (show_step_sizes) {
      std::vector<float> temp;
      float max = -1000;
      float min = +100000;
      for (int i = 0;
           i < critic_v->list_of_output_features[0]->incoming_edges.size();
           i++) {
        float mean_sub_feature = log(
            critic_v->list_of_output_features[0]->incoming_edges[i].step_size);
        if (mean_sub_feature > max) {
          max = mean_sub_feature;
        }
        if (mean_sub_feature < min)
          min = mean_sub_feature;
        temp.push_back(mean_sub_feature);
      }
      for (int i = 0; i < temp.size(); i++) {
        norm_features.push_back(uint8_t(255 * ((temp[i] - min) / (max - min))));
      }
      data = reinterpret_cast<unsigned char *>(norm_features.data());
    } else {
      data = reinterpret_cast<unsigned char *>(features.data());
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GLsizei(my_env.getWidth()),
                 GLsizei(my_env.getHeight()), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 data);

    ImGui::Begin("Renderings");
    ImGui::Image((void *)textureID,
                 ImVec2(my_env.getWidth() * 2, my_env.getHeight() * 2));
  }
  ImGui::Checkbox("Show normalized features", &show_normalized_features);
  ImGui::Checkbox("Show performance utility", &show_weights);
  ImGui::Checkbox("Show learning utility", &show_step_sizes);
  ImGui::End();
  ImGui::Begin("Environments");
  if (ImGui::BeginListBox("Games")) {
    for (int n = 0; n < my_models.size(); n++) {
      const bool is_selected = (game_id == n);
      if (ImGui::Selectable(my_models[n].c_str(), is_selected))
        game_id = n;

      if (is_selected)
        ImGui::SetItemDefaultFocus();
    }
    ImGui::EndListBox();
  }
  if (ImGui::Button("Load model")) {
    std::cout << "Loading model\n";
    my_env.loadROM(my_models[game_id]);
  }
  ImGui::End();
  ImGui::Begin("Reward");
  if (ImGui::Button("Reward")) {
    reward = 1.5;
  }
  if (ImGui::Button("Step")) {
    to_step = true;
  }
  ImGui::Text("Value = %f", S_value);
  ImGui::Text("Steps = %d", environment_steps);
  ImGui::Text("Reward = %f", reward);
  ImGui::Text("Norm = %f",
              critic_v->list_of_output_features[0]->trace_of_incoming_norm);

  ImGui::Checkbox("Disable Learning", &frozen_agent);
  ImGui::Checkbox("Disable rendering", &no_render);

  ImGui::SliderInt("Speed", &fps, 0, 50);
  ImGui::SliderInt("Slow down", &update_intervel, 1, 20);

  ImGui::Begin("Statistics");
  ImGui::Text("Environment fps = %f", fps_estimate * fps / update_intervel);
  ImGui::Text("UI fps = %f", fps_estimate);
  ImGui::End();
}

int main() {
  critic = network_initializer.initialize_sprase_networks(
      critic, 0, 10, "sigmoid", critic_step_size);

  critic = weight_initializer.initialize_weights(critic);

  opti_critic = new TIDBD(meta_step_size, 0.99, 1e-8, critic);
  //  opti_critic = new SGD(critic_step_size);
  for (const auto &entry : fs::directory_iterator("../data/")) {
    auto s = entry.path().string();
    std::cout << "Directory data " << s << std::endl;
    if (s.find(".bin") != std::string::npos) {
      std::string model_name = ".";
      for (int i = 1; i < s.size(); i++) {
        model_name += s[i];
      }
      my_models.push_back(model_name);
    }
    std::sort(my_models.begin(), my_models.end());
  }

  auto window = initialize_gui();

  // Get & Set the desired settings
  my_env.setInt("random_seed", 2147483627);
  my_env.setFloat("repeat_action_probability", 0.0);
  my_env.setInt("frame_skip", 1);

  my_env.loadROM("../data/space_invaders.bin");

  // Create one OpenGL texture
  GLuint textureID;
  glGenTextures(1, &textureID);

  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  GL_CLAMP_TO_EDGE); // This is required on WebGL for non
                                     // power-of-two textures
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

  while (!glfwWindowShouldClose(window)) {
    ui_steps++;
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < fps; i++) {
      if (ui_steps % update_intervel == 0) {
        agent_step();
      }
    }
    if (fps == 0 && to_step) {
      agent_step();
      to_step = false;
    }
    glfwPollEvents();

    add_imgui_interface(textureID);

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);

    //    Estimate performance of the UI and the environment
    std::chrono::duration<double> elapsed_seconds =
        std::chrono::system_clock::now() - start;
    fps_estimate = fps_estimate * 0.95 + 0.05 * 1.0 / (elapsed_seconds.count());
  }

  return 0;
}
