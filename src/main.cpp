#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"

void configScene();
void renderScene();
void setLights(glm::mat4 P, glm::mat4 V);
void drawObject(Model& model, Material& material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSuelo(glm::mat4 P, glm::mat4 V, glm::mat4 M);
// void drawOB(Model& model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawWheels(glm::mat4 P, glm::mat4 V);
void drawCube(glm::mat4 P, glm::mat4 V);
void drawCube2(glm::mat4 P, glm::mat4 V);

void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey(GLFWwindow* window, int key, int scancode, int action, int mods);
void funScroll(GLFWwindow* window, double xoffset, double yoffset);
void funCursorPos(GLFWwindow* window, double xPos, double yPos);

// Shaders
Shaders shaders;

// Modelos
Model sphere, plane, wheel, road, cube;

// Luces y materiales
#define NLD 1
#define NLP 1
#define NLF 2
Light lightG;
Light lightD[NLD];
Light lightP[NLP];
Light lightF[NLF];
Material mLuz;
Material ruby;
Material blackRubber, pavement, grass;

// Viewport
int w = 1000;
int h = 1000;

// Animaciones
float rotX = 0.0;
float rotY = 0.0;
float desZ = 0.0;

// Movimiento de camara
float fovy = 30.0;
float alphaX = 0.0;
float alphaY = 0.0;

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
  glfwSwapInterval(1);

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
  glfwSetScrollCallback(window, funScroll);
  glfwSetCursorPosCallback(window, funCursorPos);

  // Entramos en el bucle de renderizado
  configScene();
  while (!glfwWindowShouldClose(window)) {
    renderScene();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

void configScene() {
  // Test de profundidad
  glEnable(GL_DEPTH_TEST);

  // Shaders
  shaders.initShaders("resources/shaders/vshader.glsl", "resources/shaders/fshader.glsl");

  // Modelos
  sphere.initModel("resources/models/sphere.obj");
  plane.initModel("resources/models/plane.obj");
  wheel.initModel("resources/models/rin.obj");
  road.initModel("resources/models/road.obj");
  cube.initModel("resources/models/cube.obj");

  // Luz ambiental global
  lightG.ambient = glm::vec3(0.5, 0.5, 0.5);

  // Luces direccionales
  lightD[0].direction = glm::vec3(-1.0, 0.0, 0.0);
  lightD[0].ambient = glm::vec3(0.1, 0.1, 0.1);
  lightD[0].diffuse = glm::vec3(0.7, 0.7, 0.7);
  lightD[0].specular = glm::vec3(0.7, 0.7, 0.7);

  // Luces posicionales
  lightP[0].position = glm::vec3(0.0, 3.0, 3.0);
  lightP[0].ambient = glm::vec3(0.2, 0.2, 0.2);
  lightP[0].diffuse = glm::vec3(0.9, 0.9, 0.9);
  lightP[0].specular = glm::vec3(0.9, 0.9, 0.9);
  lightP[0].c0 = 1.00;
  lightP[0].c1 = 0.22;
  lightP[0].c2 = 0.20;

  // Luces focales
  lightF[0].position = glm::vec3(-2.0, 2.0, 5.0);
  lightF[0].direction = glm::vec3(2.0, -2.0, -5.0);
  lightF[0].ambient = glm::vec3(0.2, 0.2, 0.2);
  lightF[0].diffuse = glm::vec3(0.9, 0.9, 0.9);
  lightF[0].specular = glm::vec3(0.9, 0.9, 0.9);
  lightF[0].innerCutOff = 10.0;
  lightF[0].outerCutOff = lightF[0].innerCutOff + 5.0;
  lightF[0].c0 = 1.000;
  lightF[0].c1 = 0.090;
  lightF[0].c2 = 0.032;
  lightF[1].position = glm::vec3(2.0, 2.0, 5.0);
  lightF[1].direction = glm::vec3(-2.0, -2.0, -5.0);
  lightF[1].ambient = glm::vec3(0.2, 0.2, 0.2);
  lightF[1].diffuse = glm::vec3(0.9, 0.9, 0.9);
  lightF[1].specular = glm::vec3(0.9, 0.9, 0.9);
  lightF[1].innerCutOff = 5.0;
  lightF[1].outerCutOff = lightF[1].innerCutOff + 1.0;
  lightF[1].c0 = 1.000;
  lightF[1].c1 = 0.090;
  lightF[1].c2 = 0.032;

  // Materiales
  mLuz.ambient = glm::vec4(0.0, 0.0, 0.0, 1.0);
  mLuz.diffuse = glm::vec4(0.0, 0.0, 0.0, 1.0);
  mLuz.specular = glm::vec4(0.0, 0.0, 0.0, 1.0);
  mLuz.emissive = glm::vec4(1.0, 1.0, 1.0, 1.0);
  mLuz.shininess = 1.0;

  ruby.ambient = glm::vec4(0.174500, 0.011750, 0.011750, 0.55);
  ruby.diffuse = glm::vec4(0.614240, 0.041360, 0.041360, 0.55);
  ruby.specular = glm::vec4(0.727811, 0.626959, 0.626959, 0.55);
  ruby.emissive = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
  ruby.shininess = 76.8;

  blackRubber.ambient = glm::vec4(0.02f, 0.02f, 0.02f, 1.0f);
  blackRubber.diffuse = glm::vec4(0.01f, 0.01f, 0.01f, 1.0f);
  blackRubber.specular = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
  blackRubber.shininess = 10.0;

  pavement.ambient = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
  pavement.diffuse = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
  pavement.specular = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
  pavement.emissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  pavement.shininess = 10.0f;

  grass.ambient = glm::vec4(0.1f, 0.3f, 0.1f, 1.0f);
  grass.diffuse = glm::vec4(0.2f, 0.8f, 0.2f, 1.0f);
  grass.specular = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
  grass.emissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  grass.shininess = 5.0f;
}

void renderScene() {
  // Borramos el buffer de color
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Indicamos los shaders a utilizar
  shaders.useShaders();

  // Matriz P
  float nplane = 0.1;
  float fplane = 25.0;
  float aspect = (float)w / (float)h;
  glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

  // Matriz V
  float x = 10.0f * glm::cos(glm::radians(alphaY)) * glm::sin(glm::radians(alphaX));
  float y = 10.0f * glm::sin(glm::radians(alphaY)) + 5.0;
  float z = 10.0f * glm::cos(glm::radians(alphaY)) * glm::cos(glm::radians(alphaX)) + 5.0;
  glm::vec3 eye(x, y, z);
  glm::vec3 center(0.0, 0.0, 0.0);
  glm::vec3 up(0.0, 1.0, 0.0);
  glm::mat4 V = glm::lookAt(eye, center, up);
  shaders.setVec3("uEye", eye);

  // Fijamos las luces
  setLights(P, V);

  // Dibujamos la escena
  // Suelo
  glm::mat4 S = glm::scale(I, glm::vec3(7.0, 1.0, 5.0));
  glm::mat4 Ry = glm::rotate(I, glm::radians(rotY), glm::vec3(0, 1, 0));
  glm::mat4 Rx = glm::rotate(I, glm::radians(rotX), glm::vec3(1, 0, 0));
  glm::mat4 Tz = glm::translate(I, glm::vec3(0.0, 0.0, desZ));
  drawObject(plane, grass, P, V, Tz * Rx * Ry * S);

  // Ruedas
  // Rx = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
  // Tz = glm::translate(I, glm::vec3(0.0, 0.5, desZ));
  // S = glm::scale(I, glm::vec3(0.2, 0.2, 0.2));
  drawWheels(P, V);

  // Chasis coche
  // Rx = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
  // Tz = glm::translate(I, glm::vec3(0.0, 0.5, desZ));
  // S = glm::scale(I, glm::vec3(0.2, 0.2, 0.2));
  drawCube(P, V);

  drawCube2(P, V);

  // Carretera
  Ry = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 1, 0));
  Tz = glm::translate(I, glm::vec3(1.0, 0.05, desZ));
  S = glm::scale(I, glm::vec3(0.004, 0.008, 0.009));
  glm::mat4 Tx = glm::translate(I, glm::vec3(-1.0, 0.0, 0.0));
  drawObject(road, pavement, P, V, Tx * Tz * Ry * S);
}

void setLights(glm::mat4 P, glm::mat4 V) {
  shaders.setLight("uLightG", lightG);
  for (int i = 0; i < NLD; i++)
    shaders.setLight("uLightD[" + toString(i) + "]", lightD[i]);
  for (int i = 0; i < NLP; i++)
    shaders.setLight("uLightP[" + toString(i) + "]", lightP[i]);
  for (int i = 0; i < NLF; i++)
    shaders.setLight("uLightF[" + toString(i) + "]", lightF[i]);

  for (int i = 0; i < NLP; i++) {
    glm::mat4 M = glm::translate(I, lightP[i].position) * glm::scale(I, glm::vec3(0.1));
    drawObject(sphere, mLuz, P, V, M);
  }

  for (int i = 0; i < NLF; i++) {
    glm::mat4 M = glm::translate(I, lightF[i].position) * glm::scale(I, glm::vec3(0.025));
    drawObject(sphere, mLuz, P, V, M);
  }
}

void drawObject(Model& model, Material& material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {
  shaders.setMat4("uN", glm::transpose(glm::inverse(M)));
  shaders.setMat4("uM", M);
  shaders.setMat4("uPVM", P * V * M);
  shaders.setMaterial("uMaterial", material);
  model.renderModel(GL_FILL);
}

/*
void drawOB(Model& model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M) {  // suelo y coche
  shaders.setMat4("uPVM", P * V * M);

  glEnable(GL_POLYGON_OFFSET_FILL);
  shaders.setVec3("uColor", color);
  model.renderModel(GL_FILL);
  glDisable(GL_POLYGON_OFFSET_FILL);

  shaders.setVec3("uColor", glm::vec3(1.0, 0.0, 1.0));
  model.renderModel(GL_LINE);
}
*/

void drawSuelo(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
  glm::mat4 S = glm::scale(I, glm::vec3(5.0, 1.0, 5.0));
  glm::mat4 R = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 1, 0));
  drawObject(plane, pavement, P, V, M * S * R);
}

void drawCube(glm::mat4 P, glm::mat4 V) {
  glm::mat4 S = glm::scale(I, glm::vec3(1.8, 0.7, 0.75));
  glm::mat4 T = glm::translate(I, glm::vec3(-0.7, 1.3, -0.6));
  glm::mat4 R = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
  drawObject(cube, ruby, P, V, T * R * S);
}

void drawCube2(glm::mat4 P, glm::mat4 V) {
  glm::mat4 S = glm::scale(I, glm::vec3(1.0, 0.7, 0.75));
  glm::mat4 T = glm::translate(I, glm::vec3(-0.65, 2.3, -0.6));
  glm::mat4 R = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
  drawObject(cube, ruby, P, V, T * R * S);
}

void drawWheels(glm::mat4 P, glm::mat4 V) {
  glm::mat4 Rx = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
  glm::mat4 Tz = glm::translate(I, glm::vec3(0.45, 0.5, 0.0));
  glm::mat4 S = glm::scale(I, glm::vec3(0.2, 0.2, 0.2));
  // Rueda trasera izquierda
  drawObject(wheel, blackRubber, P, V, Tz * Rx * S);

  Tz = glm::translate(I, glm::vec3(-1.75, 0.5, 0.0));
  // Rueda delantera izquierda
  drawObject(wheel, blackRubber, P, V, Tz * Rx * S);

  glm::mat4 Ry = glm::rotate(I, glm::radians(180.0f), glm::vec3(0, 1, 0));
  glm::mat4 Trs = glm::translate(I, glm::vec3(0.0, 0.0, -1.24));
  // Rueda delantera derecha
  drawObject(wheel, blackRubber, P, V, Trs * Tz * Ry * Rx * S);

  glm::mat4 Tx = glm::translate(I, glm::vec3(0.45, 0.5, -1.0));
  // Rueda trasera derecha
  drawObject(wheel, blackRubber, P, V, Tx * Rx * S);
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
      rotX -= 5.0f;
      break;
    case GLFW_KEY_DOWN:
      rotX += 5.0f;
      break;
    case GLFW_KEY_LEFT:
      rotY -= 5.0f;
      break;
    case GLFW_KEY_RIGHT:
      rotY += 5.0f;
      break;
    case GLFW_KEY_Z:
      if (mods == GLFW_MOD_SHIFT)
        desZ -= desZ > -24.0f ? 0.1f : 0.0f;
      else
        desZ += desZ < 5.0f ? 0.1f : 0.0f;
      break;
    default:
      rotX = 0.0f;
      rotY = 0.0f;
      break;
  }
}

void funScroll(GLFWwindow* window, double xoffset, double yoffset) {
  if (yoffset > 0)
    fovy -= fovy > 10.0f ? 5.0f : 0.0f;
  if (yoffset < 0)
    fovy += fovy < 90.0f ? 5.0f : 0.0f;
}

void funCursorPos(GLFWwindow* window, double xPos, double yPos) {
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    return;

  float limY = 89.0;
  alphaX = 90.0 * (2.0 * xPos / (float)w - 1.0);
  alphaY = 90.0 * (1.0 - 2.0 * yPos / (float)h);
  if (alphaY < -limY)
    alphaY = -limY;
  if (alphaY > limY)
    alphaY = limY;
}
