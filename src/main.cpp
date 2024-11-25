#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"

void configScene();
void renderScene();
void drawObject(Model& model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSuelo(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAspa(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawHelice(glm::mat4 P, glm::mat4 V, glm::mat4 M);

void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey(GLFWwindow* window, int key, int scancode, int action, int mods);
void funCursorPos (GLFWwindow* window, double xpos, double ypos);
void funTimer(double seconds, double &start);
void funTimer2(double seconds, double &start2);
//no hay callback de timer, hay que hacerlo uno mismo.

// Shaders
Shaders shaders;

// Modelos
Model plane;
Model triangle;

// Viewport
int w = 500;
int h = 500;

// Animaciones
float desZ = 0.0;
float rotZ = 0.0;

float alphax = 0.0;
float alphay = 0.0;

float rotY = 5.0; // esto es del final de la sesion 5
float rotY2 = 2.0;

glm::vec3 up(0.0, 1.0, 0.0);

int main() {
  // Inicializamos GLFW
  if (!glfwInit())
    return -1;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Creamos la ventana
  GLFWwindow* window;
  window = glfwCreateWindow(w, h, "Proyecto Final", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // si vamos a poner animaciones poner aqui un 0:  glfwSwapInterval(0);

  // Inicializamos GLEW
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    std::cout << "Error: " << glewGetErrorString(err) << std::endl;
    return -1;
  }
  std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
  const GLubyte* oglVersion = glGetString(GL_VERSION);
  std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;

  // Configuramos los CallBacks
  glfwSetFramebufferSizeCallback(window, funFramebufferSize);
  glfwSetKeyCallback(window, funKey);
  glfwSetCursorPosCallback(window, funCursorPos);
  //glfwSetTimer(windows, funTimer, seconds);

  // Entramos en el bucle de renderizado
  configScene();
  double start = glfwGetTime();
  double start2 = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    renderScene();
    glfwSwapBuffers(window);
    glfwPollEvents();
    //  1/60.0 (en lugar del 0.5)
    // Ahora cada medio segundo da 6.0
    /*
    if (glfwGetTime()-start > 0.5  1/60 ){
        rotY -= 6.0
        start = glfwGetTime();
    }
    */
    funTimer(1.0/60, start);
    funTimer2(1.0/60, start2);  // no poner los segundos menos de 0,02; tipo 1.0/1000, no pasarse

  }
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

void funTimer(double seconds, double &start){

    if (glfwGetTime()-start > seconds /*  1/60  */){
        rotY -= 6.0;
        start = glfwGetTime();
    }
}

void funTimer2(double seconds, double &start2){

    if (glfwGetTime()-start2 > seconds /*  1/60  */){
        rotY2 -= 3.0;
        start2 = glfwGetTime();
    }
}

void configScene() {
  // Test de profundidad
  glEnable(GL_DEPTH_TEST);
  glPolygonOffset(1.0, 1.0);

  // Shaders
  shaders.initShaders("resources/shaders/vshader.glsl", "resources/shaders/fshader.glsl");

  // Modelos
  plane.initModel("resources/models/plane.obj");
  triangle.initModel("resources/models/triangle.obj");
}

void renderScene() {
  // Borramos el buffer de color
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Indicamos los shaders a utilizar
  shaders.useShaders();

  // Matriz P
  float fovy = 60.0;
  float nplane = 0.1;
  float fplane = 25.0;
  float aspect = (float)w / (float)h;
  glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

  // Matriz V
  double x = 10.0*cos(glm::radians(alphay))*sin(glm::radians(alphax));
  double y = 10.0*sin(glm::radians(alphay));
  double z = 10.0*cos(glm::radians(alphay))*cos(glm::radians(alphax));

  glm::vec3 eye(x, y, z);
  //glm::vec3 eye(0.0, 0.0, 10.0);
  glm::vec3 center(0.0, 0.0, 0.0);
  //glm::vec3 up(0.0, 1.0, 0.0);
  glm::mat4 V = glm::lookAt(eye, center, up);

  // Dibujamos la escena
  drawSuelo(P, V, I);

  glm::mat4 T = glm::translate(I, glm::vec3(0.0, 0.0, desZ));
  glm::mat4 R = glm::rotate(I, glm::radians(rotZ), glm::vec3(0, 0, 1));


  //
  //glm::mat4 R = glm::rotate(I, glm::radians(rotZ), glm::vec3(0, 0, 1));
  //
  //
  //
  //

  drawHelice(P, V, R * T);
}

void drawObject(Model& model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M) {
  shaders.setMat4("uPVM", P * V * M);

  glEnable(GL_POLYGON_OFFSET_FILL);
  shaders.setVec3("uColor", color);
  model.renderModel(GL_FILL);
  glDisable(GL_POLYGON_OFFSET_FILL);

  shaders.setVec3("uColor", glm::vec3(1.0, 1.0, 1.0));
  model.renderModel(GL_LINE);
}

void drawSuelo(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
  glm::mat4 S = glm::scale(I, glm::vec3(5.0, 1.0, 5.0));
  drawObject(plane, glm::vec3(0.0, 0.0, 1.0), P, V, M * S);
}

void drawAspa(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
  glm::mat4 T = glm::translate(I, glm::vec3(0.0, -1.0, 0.0));
  drawObject(triangle, glm::vec3(1.0, 0.0, 0.0), P, V, M * T);
}

void drawHelice(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
  glm::mat4 Rz90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 0, 1));
  drawAspa(P, V, M);
  drawAspa(P, V, M * Rz90);
  drawAspa(P, V, M * Rz90 * Rz90);
  drawAspa(P, V, M * Rz90 * Rz90 * Rz90);
}

void funFramebufferSize(GLFWwindow* window, int width, int height) {
  // Configuracion del Viewport
  glViewport(0, 0, width, height);

  // Actualizacion de w y h
  w = width;
  h = height;
}

void funKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
  switch (key) {
    case GLFW_KEY_UP:
      desZ -= 0.1;
      break;
    case GLFW_KEY_DOWN:
      desZ += 0.1;
      break;
    case GLFW_KEY_LEFT:
      rotZ += 5.0;
      break;
    case GLFW_KEY_RIGHT:
      rotZ -= 5.0;
      break;
    default:
      desZ = 0.0;
      rotZ = 0.0;
  }
}
void funCursorPos (GLFWwindow* window, double xpos, double ypos){

  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) return;
  
  alphax = 90.0*(2.0*xpos/(float)w - 1.0);
  alphay = 90.0*(1.0 - 2-0*ypos/(float)w);

  if (alphay > 89.9) alphay = 89.9;
  if (alphay < -89.9) alphay = -89.9;

  std::cout << "CursorPos " << xpos << " " << ypos << std::endl;

  //if (alphay > 89.9) up = glm::vec3(0.0, -1.0, 0.0);
                      //alphay = 89.9;

  

}


