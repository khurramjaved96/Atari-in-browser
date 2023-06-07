//
// Created by Khurram Javed on 2023-04-26.
//

#ifndef _GUI_INITIALIZATION_H_
#define _GUI_INITIALIZATION_H_
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>

GLFWwindow * initialize_gui() {
  if (!glfwInit())
    std::cout << "GLFW init not configured properly\n";
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
  GLFWwindow *window =
      glfwCreateWindow(1980, 1080, "TicTacToe Tracking", NULL, NULL);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

  ImGui::StyleColorsDark();

  io.Fonts->AddFontFromFileTTF("../data/poppin.ttf", 18.0f);
  io.Fonts->AddFontFromFileTTF("../data/poppin.ttf", 32.0f);
  io.Fonts->AddFontDefault();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);


  return window;
}
#endif //_GUI_INITIALIZATION_H_
