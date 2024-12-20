#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"
#include "Texture.h"

void configScene();
void renderScene();
void setLights(glm::mat4 P, glm::mat4 V);
void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawWheels(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCube(glm::mat4 P, Material& material, glm::mat4 V, glm::mat4 M);
void drawCube2(glm::mat4 P, Material& material, glm::mat4 V, glm::mat4 M);
void drawLights(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawWindows(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCoche(glm::mat4 P, Material& material, glm::mat4 V, glm::mat4 M);

void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey(GLFWwindow* window, int key, int scancode, int action, int mods);
void funScroll(GLFWwindow* window, double xoffset, double yoffset);
void funCursorPos(GLFWwindow* window, double xPos, double yPos);

// Shaders
Shaders shaders;

// Modelos
Model sphere, plane, wheel, road, cube;

// Imagenes (texturas)
Texture imgHighway, imgNoEmissive;
Texture imgPavingStones, imgPavingStonesNormal;

// Luces y materiales
#define NLD 1
#define NLP 1
#define NLF 6
Light lightG;
Light lightD[NLD];
Light lightP[NLP];
Light lightF[NLF];
Material mLuz, ruby;
Material blackRubber, cyanPlastic, polishedBronze, gold, pearl, emerald, jade;
Textures texHighway, texPavingStones;

// Viewport
int w = 1000;
int h = 1000;

// Animaciones

// Movimiento de camara
float fovy = 60.0;
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

  // Transparencias
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_CULL_FACE);

  // Shaders
  shaders.initShaders("resources/shaders/vshader.glsl", "resources/shaders/fshader.glsl");

  // Modelos
  sphere.initModel("resources/models/sphere.obj");
  plane.initModel("resources/models/plane.obj");
  wheel.initModel("resources/models/wheel.obj");
  road.initModel("resources/models/highway.obj");
  cube.initModel("resources/models/cube.obj");

  // Imagenes (texturas)
  imgNoEmissive.initTexture("resources/textures/imgNoEmissive.png");
  imgHighway.initTexture("resources/textures/Road007_1K_Color.jpeg");
  imgPavingStones.initTexture("resources/textures/PavingStones069_1K_Color.jpeg");
  imgPavingStonesNormal.initTexture("resources/textures/PavingStones069_1K_NormalDX.jpeg");

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

  lightF[2].position = glm::vec3(-2.5, 1.6, -1.0);
  lightF[2].direction = glm::vec3(-2.0, -3.0, 0.0);
  lightF[2].ambient = glm::vec3(0.2, 0.2, 0.2);
  lightF[2].diffuse = glm::vec3(0.9, 0.9, 0.9);
  lightF[2].specular = glm::vec3(0.9, 0.9, 0.9);
  lightF[2].innerCutOff = 10.0;
  lightF[2].outerCutOff = lightF[2].innerCutOff + 1.0;
  lightF[2].c0 = 1.000;
  lightF[2].c1 = 0.090;
  lightF[2].c2 = 0.032;

  lightF[3].position = glm::vec3(-2.5, 1.6, -0.2);
  lightF[3].direction = glm::vec3(-2.0, -3.0, 0.0);
  lightF[3].ambient = glm::vec3(0.2, 0.2, 0.2);
  lightF[3].diffuse = glm::vec3(0.9, 0.9, 0.9);
  lightF[3].specular = glm::vec3(0.9, 0.9, 0.9);
  lightF[3].innerCutOff = 10.0;
  lightF[3].outerCutOff = lightF[3].innerCutOff + 1.0;
  lightF[3].c0 = 1.000;
  lightF[3].c1 = 0.090;
  lightF[3].c2 = 0.032;

  lightF[4].position = glm::vec3(1.1, 1.6, -0.2);
  lightF[4].direction = glm::vec3(2.0, -3.0, 0.0);
  lightF[4].ambient = glm::vec3(0.2, 0.2, 0.2);
  lightF[4].diffuse = glm::vec3(0.9, 0.0, 0.0);
  lightF[4].specular = glm::vec3(0.9, 0.0, 0.0);
  lightF[4].innerCutOff = 10.0;
  lightF[4].outerCutOff = lightF[4].innerCutOff + 1.0;
  lightF[4].c0 = 1.000;
  lightF[4].c1 = 0.090;
  lightF[4].c2 = 0.032;

  lightF[5].position = glm::vec3(1.1, 1.6, -1.0);
  lightF[5].direction = glm::vec3(2.0, -3.0, 0.0);
  lightF[5].ambient = glm::vec3(0.2, 0.2, 0.2);
  lightF[5].diffuse = glm::vec3(0.9, 0.0, 0.0);
  lightF[5].specular = glm::vec3(0.9, 0.0, 0.0);
  lightF[5].innerCutOff = 10.0;
  lightF[5].outerCutOff = lightF[5].innerCutOff + 1.0;
  lightF[5].c0 = 1.000;
  lightF[5].c1 = 0.090;
  lightF[5].c2 = 0.032;

  // Materiales
  mLuz.ambient = glm::vec4(0.0, 0.0, 0.0, 0.5);
  mLuz.diffuse = glm::vec4(0.0, 0.0, 0.0, 0.5);
  mLuz.specular = glm::vec4(0.0, 0.0, 0.0, 0.5);
  mLuz.emissive = glm::vec4(1.0, 1.0, 1.0, 0.5);
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

  texHighway.diffuse = imgHighway.getTexture();
  texHighway.specular = imgHighway.getTexture();
  texHighway.emissive = imgNoEmissive.getTexture();
  texHighway.normal = 0;
  texHighway.shininess = 5.0f;

  texPavingStones.diffuse = imgPavingStones.getTexture();
  texPavingStones.specular = imgPavingStones.getTexture();
  texPavingStones.emissive = imgNoEmissive.getTexture();
  texPavingStones.normal = imgPavingStonesNormal.getTexture();
  texPavingStones.shininess = 5.0f;

  cyanPlastic.ambient = glm::vec4(0.0f, 0.1f, 0.06f, 0.5f);
  cyanPlastic.diffuse = glm::vec4(0.0f, 0.50980392f, 0.50980392f, 0.5f);
  cyanPlastic.specular = glm::vec4(0.50196078f, 0.50196078f, 0.50196078f, 0.5f);
  cyanPlastic.emissive = glm::vec4(0.0f, 0.0f, 0.0f, 0.5f);
  cyanPlastic.shininess = 32.0f;

  polishedBronze.ambient = glm::vec4(0.25f, 0.148f, 0.06475f, 1.0f);
  polishedBronze.diffuse = glm::vec4(0.4f, 0.2368f, 0.1036f, 1.0f);
  polishedBronze.specular = glm::vec4(0.774597f, 0.458561f, 0.200621f, 1.0f);
  polishedBronze.emissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  polishedBronze.shininess = 76.8f;

  gold.ambient = glm::vec4(0.329412f, 0.223529f, 0.027451f, 1.0f);
  gold.diffuse = glm::vec4(0.780392f, 0.568627f, 0.113725f, 1.0f);
  gold.specular = glm::vec4(0.992157f, 0.941176f, 0.807843f, 1.0f);
  gold.emissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  gold.shininess = 100.0f;

  pearl.ambient = glm::vec4(0.25f, 0.20725f, 0.20725f, 0.922f);
  pearl.diffuse = glm::vec4(1.0f, 0.829f, 0.829f, 0.922f);
  pearl.specular = glm::vec4(0.296648f, 0.296648f, 0.296648f, 0.922f);
  pearl.emissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  pearl.shininess = 11.264f;

  emerald.ambient = glm::vec4(0.0215f, 0.1745f, 0.0215f, 1.0f);
  emerald.diffuse = glm::vec4(0.07568f, 0.61424f, 0.07568f, 1.0f);
  emerald.specular = glm::vec4(0.633f, 0.727811f, 0.633f, 1.0f);
  emerald.emissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  emerald.shininess = 76.8f;

  jade.ambient = glm::vec4(0.135f, 0.2225f, 0.1575f, 1.0f);
  jade.diffuse = glm::vec4(0.54f, 0.89f, 0.63f, 1.0f);
  jade.specular = glm::vec4(0.316228f, 0.316228f, 0.316228f, 1.0f);
  jade.emissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  jade.shininess = 12.8f;
}

void renderScene() {
  // Borramos el buffer de color
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Indicamos los shaders a utilizar
  shaders.useShaders();

  // Matriz P
  float nplane = 0.1;
  float fplane = 60.0;
  float aspect = (float)w / (float)h;
  glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

  // Matriz V
  float x = 10.0f * glm::cos(glm::radians(alphaY)) * glm::sin(glm::radians(alphaX));
  float y = 10.0f * glm::sin(glm::radians(alphaY)) + 5.0;
  float z = -10.0f * glm::cos(glm::radians(alphaY)) * glm::cos(glm::radians(alphaX)) + -5.0;
  glm::vec3 eye(x, y, z);
  glm::vec3 center(0.0, 0.0, 0.0);
  glm::vec3 up(0.0, 1.0, 0.0);
  glm::mat4 V = glm::lookAt(eye, center, up);
  shaders.setVec3("uEye", eye);

  // Fijamos las luces
  setLights(P, V);

  // Dibujamos la escena
  // Suelo
  glm::mat4 S = glm::scale(I, glm::vec3(18.0, 1.0, 18.0));
  glm::mat4 T = glm::translate(I, glm::vec3(7.0, 0.0, 14.0));
  drawObjectTex(plane, texPavingStones, P, V, T * S);

  // Carretera principal
  glm::mat4 Tr = glm::translate(I, glm::vec3(-14.5, 0.1, 28.75));
  glm::mat4 Sr = glm::scale(I, glm::vec3(0.015, 0.016, 0.061));
  glm::mat4 Rr = glm::rotate(I, glm::radians(180.0f), glm::vec3(0, 1, 0));
  drawObjectTex(road, texHighway, P, V, Rr * Tr * Sr);
  // Coche del jugador
  glm::mat4 Rc = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 1, 0));
  glm::mat4 Tc = glm::translate(I, glm::vec3(2.8, 0.1, 6.2));
  glm::mat4 Sc = glm::scale(I, glm::vec3(0.8, 0.8, 0.8));
  drawCoche(P, gold, V, Rc * Tc * Sc);

  // Resto de coches y carreteras
  Tr = glm::translate(I, glm::vec3(-12.0, 0.11, 50));
  Rr = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 1, 0));
  drawObjectTex(road, texHighway, P, V, Rr * Tr * Sr);
  Rc = glm::rotate(I, glm::radians(180.0f), glm::vec3(0, 1, 0));
  Tc = glm::translate(I, glm::vec3(-9.0, 0.1, 5.3));
  drawCoche(P, jade, V, Tc * Rc * Sc);
  Rc = glm::rotate(I, glm::radians(0.0f), glm::vec3(0, 1, 0));
  Tc = glm::translate(I, glm::vec3(23.0, 0.1, 3.5));
  drawCoche(P, pearl, V, Tc * Rc * Sc);

  Tr = glm::translate(I, glm::vec3(-25.0, 0.11, 50));
  drawObjectTex(road, texHighway, P, V, Rr * Tr * Sr);
  Rc = glm::rotate(I, glm::radians(180.0f), glm::vec3(0, 1, 0));
  Tc = glm::translate(I, glm::vec3(-9.0, 0.1, 18.2));
  drawCoche(P, emerald, V, Tc * Rc * Sc);
  Rc = glm::rotate(I, glm::radians(0.0f), glm::vec3(0, 1, 0));
  Tc = glm::translate(I, glm::vec3(23.0, 0.1, 16.4));
  drawCoche(P, polishedBronze, V, Tc * Rc * Sc);
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
    drawObjectMat(sphere, mLuz, P, V, M);
  }

  for (int i = 0; i < NLF; i++) {
    glm::mat4 M = glm::translate(I, lightF[i].position) * glm::scale(I, glm::vec3(0.025));
    drawObjectMat(sphere, mLuz, P, V, M);
  }
}

void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {
  shaders.setMat4("uN", glm::transpose(glm::inverse(M)));
  shaders.setMat4("uM", M);
  shaders.setMat4("uPVM", P * V * M);
  shaders.setBool("uWithMaterials", true);
  shaders.setMaterial("uMaterial", material);
  model.renderModel(GL_FILL);
}

void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M) {
  shaders.setMat4("uN", glm::transpose(glm::inverse(M)));
  shaders.setMat4("uM", M);
  shaders.setMat4("uPVM", P * V * M);
  shaders.setBool("uWithMaterials", false);
  shaders.setTextures("uTextures", textures);
  if (textures.normal != 0)
    shaders.setBool("uWithNormals", true);
  else
    shaders.setBool("uWithNormals", false);
  model.renderModel(GL_FILL);
}

void drawCube(glm::mat4 P, Material& material, glm::mat4 V, glm::mat4 M) {
  glm::mat4 S = glm::scale(I, glm::vec3(1.8, 1.0, 0.55));
  glm::mat4 T = glm::translate(I, glm::vec3(-0.7, 1.3, -0.6));
  glm::mat4 R = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
  drawObjectMat(cube, material, P, V, M * T * R * S);
}

void drawCube2(glm::mat4 P, Material& material, glm::mat4 V, glm::mat4 M) {
  glm::mat4 S = glm::scale(I, glm::vec3(1.0, 1.0, 0.75));
  glm::mat4 T = glm::translate(I, glm::vec3(-0.65, 2.0, -0.6));
  glm::mat4 R = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
  drawObjectMat(cube, material, P, V, M * T * R * S);
}

void drawWheels(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
  glm::mat4 Rx = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
  glm::mat4 Tz = glm::translate(I, glm::vec3(0.45, 0.5, 0.3));
  glm::mat4 S = glm::scale(I, glm::vec3(0.2, 0.2, 0.2));
  // Rueda trasera izquierda
  drawObjectMat(wheel, blackRubber, P, V, M * Tz * Rx * S);

  Tz = glm::translate(I, glm::vec3(-1.75, 0.5, 0.3));
  // Rueda delantera izquierda
  drawObjectMat(wheel, blackRubber, P, V, M * Tz * Rx * S);

  glm::mat4 Ry = glm::rotate(I, glm::radians(180.0f), glm::vec3(0, 1, 0));
  glm::mat4 Trs = glm::translate(I, glm::vec3(0.0, 0.0, -1.84));
  // Rueda delantera derecha
  drawObjectMat(wheel, blackRubber, P, V, M * Trs * Tz * Ry * Rx * S);

  glm::mat4 Tx = glm::translate(I, glm::vec3(0.45, 0.5, -1.3));
  // Rueda trasera derecha
  drawObjectMat(wheel, blackRubber, P, V, M * Tx * Rx * S);
}

void drawLights(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
  glm::mat4 S = glm::scale(I, glm::vec3(0.1, 0.1, 0.1));
  glm::mat4 T = glm::translate(I, glm::vec3(-2.5, 1.6, -1.0));
  glm::mat4 R = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
  glDepthMask(GL_FALSE);
  // Luces delanteras
  drawObjectMat(sphere, mLuz, P, V, M * T * R * S);
  T = glm::translate(I, glm::vec3(-2.5, 1.6, -0.2));
  drawObjectMat(sphere, mLuz, P, V, M * T * R * S);
  // Luces traseras
  T = glm::translate(I, glm::vec3(1.1, 1.6, -0.2));
  drawObjectMat(sphere, ruby, P, V, M * T * R * S);
  T = glm::translate(I, glm::vec3(1.1, 1.6, -1.0));
  drawObjectMat(sphere, ruby, P, V, M * T * R * S);
  glDepthMask(GL_TRUE);
}

void drawWindows(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
  glm::mat4 S = glm::scale(I, glm::vec3(0.02, 0.9, 0.35));
  glm::mat4 T = glm::translate(I, glm::vec3(-1.65, 2.3, -0.6));
  glm::mat4 R = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
  glDepthMask(GL_FALSE);
  // Luna delantera
  drawObjectMat(cube, cyanPlastic, P, V, M * T * R * S);
  // Luna trasera
  T = glm::translate(I, glm::vec3(0.35, 2.3, -0.6));
  drawObjectMat(cube, cyanPlastic, P, V, M * T * R * S);

  // Ventanilla delantera izquierda
  S = glm::scale(I, glm::vec3(0.35, 0.02, 0.35));
  T = glm::translate(I, glm::vec3(-1.15, 2.3, 0.4));
  drawObjectMat(cube, cyanPlastic, P, V, M * T * R * S);
  // Ventanilla trasera izquierda
  T = glm::translate(I, glm::vec3(-0.15, 2.3, 0.4));
  drawObjectMat(cube, cyanPlastic, P, V, M * T * R * S);
  // Ventanilla delantera derecha
  T = glm::translate(I, glm::vec3(-1.15, 2.3, -1.6));
  drawObjectMat(cube, cyanPlastic, P, V, M * T * R * S);
  // Ventanilla trasera derecha
  T = glm::translate(I, glm::vec3(-0.15, 2.3, -1.6));
  drawObjectMat(cube, cyanPlastic, P, V, M * T * R * S);
  glDepthMask(GL_TRUE);
}

void drawCoche(glm::mat4 P, Material& material, glm::mat4 V, glm::mat4 M) {
  // Ruedas
  drawWheels(P, V, M);
  // Chasis coche
  drawCube(P, material, V, M);
  drawCube2(P, material, V, M);
  // Luces
  drawLights(P, V, M);
  // Ventanillas
  drawWindows(P, V, M);
}

void funFramebufferSize(GLFWwindow* window, int width, int height) {
  // Configuracion del Viewport
  glViewport(0, 0, width, height);

  // Actualizacion de w y h
  w = width;
  h = height;
}

void funKey(GLFWwindow* window, int key, int scancode, int action, int mods) {}

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