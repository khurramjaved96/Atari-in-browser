#include "src/ale_interface.hpp"
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


#define GLFW_INCLUDE_ES3
#include "GLFW/glfw3.h"
#include <GLES3/gl3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"



using namespace std;

ale::ALEInterface my_env;





GLFWwindow *g_window;
ImVec4 clear_color =
    ImVec4(86.0f / 255.0f, 157.0f / 255.0f, 170.0f / 255.0f, 1.00f);
bool show_demo_window = true;
bool show_another_window = false;
int g_width;
int g_height;

// Function used by c++ to get the size of the html canvas
EM_JS(int, canvas_get_width, (), { return Module.canvas.width; });

// Function used by c++ to get the size of the html canvas
EM_JS(int, canvas_get_height, (), { return Module.canvas.height; });

// Function called by javascript
EM_JS(void, resizeCanvas, (), { js_resizeCanvas(); });

void on_size_changed() {
  glfwSetWindowSize(g_window, g_width, g_height);

  ImGui::SetCurrentContext(ImGui::GetCurrentContext());
}


long long int steps = 0;

void loop() {
  // Get the vector of legal actions
  ale::ActionVect legal_actions = my_env.getLegalActionSet();

  if(my_env.game_over()) {
    my_env.reset_game();
    cout << "Episode ended\n";
  }
  ale::Action a = legal_actions[rand() % legal_actions.size()];
  float reward = my_env.act(a);

  int width = canvas_get_width();
  int height = canvas_get_height();

  if (width != g_width || height != g_height) {
    g_width = width;
    g_height = height;
    on_size_changed();
  }

  glfwPollEvents();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Render();

  int display_w, display_h;
  glfwMakeContextCurrent(g_window);
  glfwGetFramebufferSize(g_window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwMakeContextCurrent(g_window);

}



int init_gl()
{
  if( !glfwInit() )
  {
    fprintf( stderr, "Failed to initialize GLFW\n" );
    return 1;
  }

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

  // Open a window and create its OpenGL context
  int canvasWidth = g_width;
  int canvasHeight = g_height;
  g_window = glfwCreateWindow(canvasWidth, canvasHeight, "WebGui Demo", NULL, NULL);
  if( g_window == NULL )
  {
    fprintf( stderr, "Failed to open GLFW window.\n" );
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(g_window); // Initialize GLEW

  return 0;
}



int init_imgui()
{
  // Setup Dear ImGui binding
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(g_window, true);
  ImGui_ImplOpenGL3_Init();

  // Setup style
  ImGui::StyleColorsDark();

  ImGuiIO& io = ImGui::GetIO();

  // Load Fonts
  io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 23.0f);
  io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 18.0f);
  io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 26.0f);
  io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 32.0f);
  io.Fonts->AddFontDefault();

  resizeCanvas();

  return 0;
}


int init()
{
  init_gl();
  init_imgui();
  return 0;
}


void quit()
{
  glfwTerminate();
}




extern "C" int main(int argc, char **argv) {

  g_width = canvas_get_width();
  g_height = canvas_get_height();
  if (init() != 0) return 1;

  // Get & Set the desired settings
  my_env.setInt("random_seed", 123);
  // The default is already 0.25, this is just an example
  my_env.setFloat("repeat_action_probability", 0.25);


  // Load the ROM file. (Also resets the system for new settings to
  // take effect.)
  std::cout << "Loading rom from /data/breakout.bin" << std::endl;;
  my_env.loadROM("/data/breakout.bin");
  std::cout << "Rom loaded\n";
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(loop, 0, 1);
#endif

  return 0;
}


//
//int main(int argc, char **argv) {
//  if (argc < 2) {
//    std::cerr << "Usage: " << argv[0] << " rom_file" << std::endl;
//    return 1;
//  }
//
//  ale::ALEInterface ale;
//
//  // Get & Set the desired settings
//  ale.setInt("random_seed", 123);
//  // The default is already 0.25, this is just an example
//  ale.setFloat("repeat_action_probability", 0.25);
//
//
//  // Load the ROM file. (Also resets the system for new settings to
//  // take effect.)
//  ale.loadROM(argv[1]);
//
//  // Get the vector of legal actions
//  ale::ActionVect legal_actions = ale.getLegalActionSet();
//
//  // Play 10 episodes
//  int steps = 0;
//  for (int episode = 0; episode < 10; episode++) {
//    float totalReward = 0;
//    std::vector<unsigned char> display;
//    while (!ale.game_over()) {
//      steps+=1;
//      ale::Action a = legal_actions[rand() % legal_actions.size()];
//
//      ale.getScreenGrayscale(display);
//
////      std::cout << "Width = "<< ale.environment->getScreen().width()  << " Height = " << ale.environment->getScreen().height() << std::endl;
////      std::cout << "Features size = " << display.size() << std::endl;
//      // Apply the action and get the resulting reward
//      float reward = ale.act(a);
//      totalReward += reward;
//    }
//    cout << "Episode " << episode << " ended with score: " << totalReward
//         << endl;
//    ale.reset_game();
//  }
//  std::cout << "total steps = " << steps << std::endl;
//
//  return 0;
//}
