#include "src/ale_interface.hpp"
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

using namespace std;

ale::ALEInterface my_env;
long long int steps = 0;

std::vector<unsigned char> features;
void loop() {
  // Get the vector of legal actions
  ale::ActionVect legal_actions = my_env.getLegalActionSet();

  if (my_env.game_over()) {
    my_env.reset_game();
  }
  ale::Action a = legal_actions[rand() % legal_actions.size()];
  float reward = my_env.act(a);
  my_env.getScreenGrayscale(features);

  steps++;
}

int main() {

  if (!glfwInit())
    return 1;
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
  GLFWwindow *window =
      glfwCreateWindow(1280, 720, "TicTacToe Tracking", NULL, NULL);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

  ImGui::StyleColorsDark();

  //  io.Fonts->AddFontFromFileTTF("data/poppin.ttf", 23.0f);
  //  io.Fonts->AddFontFromFileTTF("data/poppin.ttf", 18.0f);
  io.Fonts->AddFontFromFileTTF("../data/poppin.ttf", 26.0f);
  io.Fonts->AddFontFromFileTTF("../data/poppin.ttf", 32.0f);
  io.Fonts->AddFontDefault();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Get & Set the desired settings
  my_env.setInt("random_seed", 123);
  // The default is already 0.25, this is just an example
  my_env.setFloat("repeat_action_probability", 0.25);

  // Load the ROM file. (Also resets the system for new settings to
  // take effect.)
  std::cout << "Loading rom from /data/breakout.bin" << std::endl;
  ;
  my_env.loadROM("../data/breakout.bin");
  std::cout << "Rom loaded\n";

  while (!glfwWindowShouldClose(window)) {
    for (int i = 0; i < 100; i++) {
      loop();
    }
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Opponents");
    ImGui::End();
    ImGui::Text("Steps = %d", steps);
    ImGui::Text("Features = %d", features.size());
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  return 0;
}
