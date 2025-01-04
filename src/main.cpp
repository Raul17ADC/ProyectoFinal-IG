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
void drawWheels2(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCube(glm::mat4 P, Material& material, glm::mat4 V, glm::mat4 M);
void drawCube2(glm::mat4 P, Material& material, glm::mat4 V, glm::mat4 M);
void drawLights(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawWindows(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCoche(glm::mat4 P, Material& material, glm::mat4 V, glm::mat4 M);
void drawCocheD(glm::mat4 P, Material& material, glm::mat4 V, glm::mat4 M);
void funTimer(double seconds, double& start);
void funTimer2(double seconds, double& start2);
void funTimer3(double seconds, double& start3);
void funTimer4(double seconds, double& start4);
void funTimer5(double seconds, double& start5);
void funTimer6(double seconds, double& start6);
void funTimer7(double seconds, double& start7);
void funTimer8(double seconds, double& start8);
bool checkCollision(double x1, double z1, double x2, double z2, double size);

void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey(GLFWwindow* window, int key, int scancode, int action, int mods);
void funScroll(GLFWwindow* window, double xoffset, double yoffset);
void funCursorPos(GLFWwindow* window, double xPos, double yPos);

// Shaders
Shaders shaders;

// Modelos
Model sphere, plane, wheel, highway, cube, streetlight, openCube;

// Imagenes (texturas)
Texture imgHighway, imgNoEmissive;
Texture imgPavingStones, imgPavingStonesNormal;
Texture imgStreetlight, imgStreetlightSpecular;

// Luces y materiales
#define NLD 1
#define NLP 8
#define NLF 10
Light lightG;
Light lightD[NLD];
Light lightP[NLP];
Light lightF[NLF];
Material mLuz, ruby;
Material blackRubber, cyanPlastic, polishedBronze, gold, pearl, emerald, jade, pavement;
Textures texHighway, texPavingStones, texStreetlight;

// Viewport
int w = 1000;
int h = 1000;

// Movimiento de camara
float fovy = 60.0;
float alphaX = 0.0;
float alphaY = 0.0;

// Interaccion con las luces
float onOff = 1.0;
float freno = 0.0;

// Posicion coches y luces
float posDoradoZ = -2.8;
float posDoradoX = 6.2;
float posVerde = 23.0;
float posMarron = 23.0;
float posGris = -9.0;
float posJade = -9.0;
float posLuzDoradoDelantera1X = 5.4;
float posLuzDoradoDelantera1Z = -0.75;
float posLuzDoradoDelantera2X = 6.05;
float posLuzDoradoDelantera2Z = -0.75;
float posLuzDoradoTrasera1X = 5.4;
float posLuzDoradoTrasera1Z = -3.75;
float posLuzDoradoTrasera2X = 6.05;
float posLuzDoradoTrasera2Z = -3.75;
float posLuzJade = -7.0;
float posLuzVerde = -7.0;
float posLuzGris = 21.0;
float posLuzMarron = 21.0;

float ruedaDerecha = 0.0f;
float ruedaIzquierda = 0.0f;

float rotCoches = 0.0f;
float rotCocheDorado = 0.0f;

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
  double start = glfwGetTime();
  double start2 = glfwGetTime();
  double start3 = glfwGetTime();
  double start4 = glfwGetTime();
  double start5 = glfwGetTime();
  double start6 = glfwGetTime();
  double start7 = glfwGetTime();
  double start8 = glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    renderScene();
    glfwSwapBuffers(window);
    glfwPollEvents();
    // timers creados para las rotaciones de las ruedas y movimiento de los coches que NO controlamos
    // estos timers podrian agruparse siendo menos, pero por ahora lo dejamos asi por si hubieran cambios saber
    // que parte hace referencia a que coche
    funTimer(1.0 / 60, start);
    funTimer2(1.0 / 60, start2);
    funTimer3(1.0 / 60, start3);
    funTimer4(1.0 / 60, start4);
    funTimer5(1.0 / 60, start5);
    funTimer6(1.0 / 60, start6);
    funTimer7(1.0 / 60, start7);
    funTimer8(1.0 / 60, start8);
    // Restablece la posición inicial del coche Dorad

    // El 8.0, habria que modificarlo por la posicion Z de los respectivos coches
    // El 10.0 es el size de los coches que es igual para todos, hay que calcularlo. (mirar final del codigo que esta
    // ahi el metodo)

    if (checkCollision(posDoradoX, posDoradoZ, posJade, 5.3, 3.0) ||
        checkCollision(posDoradoX, posDoradoZ, posGris, 18.2, 3.0) ||
        checkCollision(posDoradoX, posDoradoZ, posMarron, 16.4, 3.0) ||
        checkCollision(posDoradoX, posDoradoZ, posVerde, 3.5, 3.0)) {
      posLuzDoradoDelantera1X = 5.4;
      posLuzDoradoDelantera1Z = -0.75;
      posLuzDoradoDelantera2X = 6.05;
      posLuzDoradoDelantera2Z = -0.75;
      posLuzDoradoTrasera1X = 5.4;
      posLuzDoradoTrasera1Z = -3.75;
      posLuzDoradoTrasera2X = 6.05;
      posLuzDoradoTrasera2Z = -3.75;
      posDoradoZ = -2.8;
      posDoradoX = 6.2;
    }
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
  highway.initModel("resources/models/highway.obj");
  cube.initModel("resources/models/cube.obj");
  streetlight.initModel("resources/models/streetlight.obj");
  openCube.initModel("resources/models/openCube.obj");

  // Imagenes (texturas)
  imgNoEmissive.initTexture("resources/textures/imgNoEmissive.png");
  imgHighway.initTexture("resources/textures/Road007_1K_Color.jpeg");
  imgPavingStones.initTexture("resources/textures/PavingStones069_1K_Color.jpeg");
  imgPavingStonesNormal.initTexture("resources/textures/PavingStones069_1K_NormalDX.jpeg");
  imgStreetlight.initTexture("resources/textures/farola_initialShadingGroup_BaseColor.png");
  imgStreetlightSpecular.initTexture("resources/textures/farola_initialShadingGroup_Roughness.png");

  // Luz ambiental global
  lightG.ambient = glm::vec3(0.5, 0.5, 0.5);

  // Luces direccionales
  lightD[0].direction = glm::normalize(glm::vec3(-0.3, -1.0, -0.2));
  lightD[0].ambient = glm::vec3(0.1, 0.1, 0.1);
  lightD[0].diffuse = glm::vec3(0.5, 0.5, 0.5);
  lightD[0].specular = glm::vec3(0.5, 0.5, 0.5);

  // Luces posicionales
  for (int i = 0; i <= 5; i++) {
    lightP[i].c0 = 1.0;
    lightP[i].c1 = 0.09;
    lightP[i].c2 = 0.032;
  }
  // Luces de freno coche jugador
  for (int j = 6; j < NLP; j++) {
    lightP[j].c0 = 1.0;
    lightP[j].c1 = 0.35;
    lightP[j].c2 = 0.44;
  }

  // Luces focales
  // Luces faros delanteros coche jugador
  lightF[0].direction = glm::vec3(0.0, -2.0, 5.0);
  lightF[0].ambient = glm::vec3(0.2, 0.2, 0.2);
  lightF[0].diffuse = glm::vec3(0.9, 0.9, 0.9);
  lightF[0].specular = glm::vec3(0.9, 0.9, 0.9);
  lightF[0].innerCutOff = 10.0;
  lightF[0].outerCutOff = lightF[0].innerCutOff + 5.0;
  lightF[0].c0 = 1.000;
  lightF[0].c1 = 0.090;
  lightF[0].c2 = 0.032;

  lightF[1].direction = glm::vec3(0.0, -2.0, 5.0);
  lightF[1].ambient = glm::vec3(0.4, 0.4, 0.4);
  lightF[1].diffuse = glm::vec3(0.9, 0.9, 0.9);
  lightF[1].specular = glm::vec3(0.9, 0.9, 0.9);
  lightF[1].innerCutOff = 10.0;
  lightF[1].outerCutOff = lightF[1].innerCutOff + 5.0;
  lightF[1].c0 = 1.000;
  lightF[1].c1 = 0.090;
  lightF[1].c2 = 0.032;

  // Coche jade, luz izquierda delantera
  lightF[2].direction = glm::vec3(5.0, -2.0, 0.0);
  lightF[2].ambient = glm::vec3(0.2, 0.2, 0.2);
  lightF[2].diffuse = glm::vec3(0.9, 0.9, 0.9);
  lightF[2].specular = glm::vec3(0.9, 0.9, 0.9);
  lightF[2].innerCutOff = 10.0;
  lightF[2].outerCutOff = lightF[2].innerCutOff + 5.0;
  lightF[2].c0 = 1.000;
  lightF[2].c1 = 0.090;
  lightF[2].c2 = 0.032;

  // Coche jade, luz derecha delantera
  lightF[3].direction = glm::vec3(5.0, -2.0, 0.0);
  lightF[3].ambient = glm::vec3(0.2, 0.2, 0.2);
  lightF[3].diffuse = glm::vec3(0.9, 0.9, 0.9);
  lightF[3].specular = glm::vec3(0.9, 0.9, 0.9);
  lightF[3].innerCutOff = 10.0;
  lightF[3].outerCutOff = lightF[3].innerCutOff + 5.0;
  lightF[3].c0 = 1.000;
  lightF[3].c1 = 0.090;
  lightF[3].c2 = 0.032;

  // Coche gris, luz izquierda delantera
  lightF[4].direction = glm::vec3(-5.0, -2.0, 1.0);
  lightF[4].ambient = glm::vec3(0.2, 0.2, 0.2);
  lightF[4].diffuse = glm::vec3(0.9, 0.9, 0.9);
  lightF[4].specular = glm::vec3(0.9, 0.9, 0.9);
  lightF[4].innerCutOff = 10.0;
  lightF[4].outerCutOff = lightF[4].innerCutOff + 5.0;
  lightF[4].c0 = 1.000;
  lightF[4].c1 = 0.090;
  lightF[4].c2 = 0.032;

  // Coche gris, luz derecha delantera
  lightF[5].direction = glm::vec3(-5.0, -2.0, 1.0);
  lightF[5].ambient = glm::vec3(0.2, 0.2, 0.2);
  lightF[5].diffuse = glm::vec3(0.9, 0.9, 0.9);
  lightF[5].specular = glm::vec3(0.9, 0.9, 0.9);
  lightF[5].innerCutOff = 10.0;
  lightF[5].outerCutOff = lightF[5].innerCutOff + 5.0;
  lightF[5].c0 = 1.000;
  lightF[5].c1 = 0.090;
  lightF[5].c2 = 0.032;

  // Coche verde, luz izquierda delantera
  lightF[6].direction = glm::vec3(5.0, -2.0, 1.0);
  lightF[6].ambient = glm::vec3(0.2, 0.2, 0.2);
  lightF[6].diffuse = glm::vec3(0.9, 0.9, 0.9);
  lightF[6].specular = glm::vec3(0.9, 0.9, 0.9);
  lightF[6].innerCutOff = 10.0;
  lightF[6].outerCutOff = lightF[6].innerCutOff + 5.0;
  lightF[6].c0 = 1.000;
  lightF[6].c1 = 0.090;
  lightF[6].c2 = 0.032;

  // Coche verde, luz derecha delantera
  lightF[7].direction = glm::vec3(5.0, -2.0, 1.0);
  lightF[7].ambient = glm::vec3(0.2, 0.2, 0.2);
  lightF[7].diffuse = glm::vec3(0.9, 0.9, 0.9);
  lightF[7].specular = glm::vec3(0.9, 0.9, 0.9);
  lightF[7].innerCutOff = 10.0;
  lightF[7].outerCutOff = lightF[7].innerCutOff + 5.0;
  lightF[7].c0 = 1.000;
  lightF[7].c1 = 0.090;
  lightF[7].c2 = 0.032;

  // Coche marron, luz delantera derecha
  lightF[8].direction = glm::vec3(-5.0, -2.0, 1.0);
  lightF[8].ambient = glm::vec3(0.2, 0.2, 0.2);
  lightF[8].diffuse = glm::vec3(0.9, 0.9, 0.9);
  lightF[8].specular = glm::vec3(0.9, 0.9, 0.9);
  lightF[8].innerCutOff = 10.0;
  lightF[8].outerCutOff = lightF[8].innerCutOff + 5.0;
  lightF[8].c0 = 1.000;
  lightF[8].c1 = 0.090;
  lightF[8].c2 = 0.032;

  // Coche marron, luz delantera izquierda
  lightF[9].direction = glm::vec3(-5.0, -2.0, 1.0);
  lightF[9].ambient = glm::vec3(0.2, 0.2, 0.2);
  lightF[9].diffuse = glm::vec3(0.9, 0.9, 0.9);
  lightF[9].specular = glm::vec3(0.9, 0.9, 0.9);
  lightF[9].innerCutOff = 10.0;
  lightF[9].outerCutOff = lightF[9].innerCutOff + 5.0;
  lightF[9].c0 = 1.000;
  lightF[9].c1 = 0.090;
  lightF[9].c2 = 0.032;

  // Materiales y texturas
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

  blackRubber.ambient = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
  blackRubber.diffuse = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
  blackRubber.specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
  blackRubber.shininess = 32.0f;

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

  pearl.ambient = glm::vec4(0.25f, 0.20725f, 0.20725f, 1.0f);
  pearl.diffuse = glm::vec4(1.0f, 0.829f, 0.829f, 1.0f);
  pearl.specular = glm::vec4(0.296648f, 0.296648f, 0.296648f, 1.0f);
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

  pavement.ambient = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f);
  pavement.diffuse = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
  pavement.specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
  pavement.emissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  pavement.shininess = 32.0f;

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

  texStreetlight.diffuse = imgStreetlight.getTexture();
  texStreetlight.specular = imgStreetlightSpecular.getTexture();
  texStreetlight.emissive = imgNoEmissive.getTexture();
  texStreetlight.normal = 0;
  texStreetlight.shininess = 50.0f;
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
  float x = 10.0f * glm::cos(glm::radians(alphaY)) * glm::sin(glm::radians(alphaX)) + 6.0;
  float y = 10.0f * glm::sin(glm::radians(alphaY)) + 5.0;
  float z = -10.0f * glm::cos(glm::radians(alphaY)) * glm::cos(glm::radians(alphaX)) + -5.0;
  glm::vec3 eye(x, y, z);
  glm::vec3 center(5.5, 1.0, 5.0);
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
  glm::mat4 R = glm::rotate(I, glm::radians(180.0f), glm::vec3(1, 0, 0));
  T = glm::translate(I, glm::vec3(7.0, -0.1, 14.0));
  drawObjectTex(plane, texPavingStones, P, V, T * R * S);

  // Carretera principal
  glm::mat4 Tr = glm::translate(I, glm::vec3(-14.5, 0.1, 28.75));
  glm::mat4 Sr = glm::scale(I, glm::vec3(0.015, 0.016, 0.061));
  glm::mat4 Rr = glm::rotate(I, glm::radians(180.0f), glm::vec3(0, 1, 0));
  drawObjectMat(highway, pavement, P, V, Rr * Tr * Sr);

  // Coche del jugador
  glm::mat4 Rc = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 1, 0));
  glm::mat4 Tc = glm::translate(I, glm::vec3(posDoradoX, 0.1, posDoradoZ));
  glm::mat4 Sc = glm::scale(I, glm::vec3(0.8, 0.8, 0.8));
  drawCocheD(P, gold, V, Tc * Rc * Sc);

  // Resto de coches y carreteras
  Tr = glm::translate(I, glm::vec3(-2.9, 0.01, 35.8));
  Rr = glm::rotate(I, glm::radians(270.0f), glm::vec3(0, 1, 0));
  drawObjectMat(highway, pavement, P, V, Rr * Tr * Sr);
  Rc = glm::rotate(I, glm::radians(180.0f), glm::vec3(0, 1, 0));
  Tc = glm::translate(I, glm::vec3(posJade, 0.1, 5.3));
  drawCoche(P, jade, V, Tc * Rc * Sc);
  Rc = glm::rotate(I, glm::radians(0.0f), glm::vec3(0, 1, 0));
  Tc = glm::translate(I, glm::vec3(posVerde, 0.1, 3.5));
  drawCoche(P, emerald, V, Tc * Rc * Sc);

  Tr = glm::translate(I, glm::vec3(10.0, 0.01, 35.8));
  drawObjectMat(highway, pavement, P, V, Rr * Tr * Sr);
  Rc = glm::rotate(I, glm::radians(180.0f), glm::vec3(0, 1, 0));
  Tc = glm::translate(I, glm::vec3(posGris, 0.1, 18.2));
  drawCoche(P, pearl, V, Tc * Rc * Sc);
  Rc = glm::rotate(I, glm::radians(0.0f), glm::vec3(0, 1, 0));
  Tc = glm::translate(I, glm::vec3(posMarron, 0.1, 16.4));
  drawCoche(P, polishedBronze, V, Tc * Rc * Sc);

  // Farolas
  glm::mat4 Tf = glm::translate(I, glm::vec3(3.5, 0.1, -2.5));
  glm::mat4 Sf = glm::scale(I, glm::vec3(0.3, 0.3, 0.3));
  drawObjectTex(streetlight, texStreetlight, P, V, Tf * Sf);
  Tf = glm::translate(I, glm::vec3(10.5, 0.1, -2.5));
  drawObjectTex(streetlight, texStreetlight, P, V, Tf * Sf);
  Tf = glm::translate(I, glm::vec3(3.5, 0.1, 9.5));
  drawObjectTex(streetlight, texStreetlight, P, V, Tf * Sf);
  Tf = glm::translate(I, glm::vec3(10.5, 0.1, 9.5));
  drawObjectTex(streetlight, texStreetlight, P, V, Tf * Sf);
  Tf = glm::translate(I, glm::vec3(3.5, 0.1, 24.5));
  drawObjectTex(streetlight, texStreetlight, P, V, Tf * Sf);
  Tf = glm::translate(I, glm::vec3(10.5, 0.1, 24.5));
  drawObjectTex(streetlight, texStreetlight, P, V, Tf * Sf);
}

void setLights(glm::mat4 P, glm::mat4 V) {
  // Luces coches
  // Coche jugador
  lightF[0].position = glm::vec3(posLuzDoradoDelantera1X, 1.4, posLuzDoradoDelantera1Z);
  lightF[1].position = glm::vec3(posLuzDoradoDelantera2X, 1.4, posLuzDoradoDelantera2Z);
  lightP[6].position = freno * glm::vec3(posLuzDoradoTrasera1X, 1.4, posLuzDoradoTrasera1Z);
  lightP[6].ambient = freno * glm::vec3(0.5, 0.0, 0.0);
  lightP[6].diffuse = freno * glm::vec3(1.0, 0.0, 0.0);
  lightP[6].specular = freno * glm::vec3(1.0, 0.0, 0.0);
  lightP[7].position = freno * glm::vec3(posLuzDoradoTrasera2X, 1.4, posLuzDoradoTrasera2Z);
  lightP[7].ambient = freno * glm::vec3(0.5, 0.0, 0.0);
  lightP[7].diffuse = freno * glm::vec3(1.0, 0.0, 0.0);
  lightP[7].specular = freno * glm::vec3(1.0, 0.0, 0.0);

  // Jade
  lightF[2].position = glm::vec3(posLuzJade, 1.4, 5.45);
  lightF[3].position = glm::vec3(posLuzJade, 1.4, 6.1);

  // Gris
  lightF[4].position = glm::vec3(posLuzGris, 1.4, 3.35);
  lightF[5].position = glm::vec3(posLuzGris, 1.4, 2.7);

  // Verde
  lightF[6].position = glm::vec3(posLuzVerde, 1.4, 18.36);
  lightF[7].position = glm::vec3(posLuzVerde, 1.4, 18.98);

  // Marron
  lightF[8].position = glm::vec3(posLuzMarron, 1.4, 15.62);
  lightF[9].position = glm::vec3(posLuzMarron, 1.4, 16.28);

  // Luces farolas
  lightP[0].position = onOff * glm::vec3(3.5, 4.35, -1.5);
  lightP[0].ambient = onOff * glm::vec3(0.3, 0.3, 0.3);
  lightP[0].diffuse = onOff * glm::vec3(1.0, 1.0, 1.0);
  lightP[0].specular = onOff * glm::vec3(1.0, 1.0, 1.0);

  lightP[1].position = onOff * glm::vec3(10.5, 4.35, -1.5);
  lightP[1].ambient = onOff * glm::vec3(0.3, 0.3, 0.3);
  lightP[1].diffuse = onOff * glm::vec3(1.0, 1.0, 1.0);
  lightP[1].specular = onOff * glm::vec3(1.0, 1.0, 1.0);

  lightP[2].position = onOff * glm::vec3(3.5, 4.35, 10.5);
  lightP[2].ambient = onOff * glm::vec3(0.3, 0.3, 0.3);
  lightP[2].diffuse = onOff * glm::vec3(1.0, 1.0, 1.0);
  lightP[2].specular = onOff * glm::vec3(1.0, 1.0, 1.0);

  lightP[3].position = onOff * glm::vec3(10.5, 4.35, 10.5);
  lightP[3].ambient = onOff * glm::vec3(0.3, 0.3, 0.3);
  lightP[3].diffuse = onOff * glm::vec3(1.0, 1.0, 1.0);
  lightP[3].specular = onOff * glm::vec3(1.0, 1.0, 1.0);

  lightP[4].position = onOff * glm::vec3(3.5, 4.35, 25.5);
  lightP[4].ambient = onOff * glm::vec3(0.3, 0.3, 0.3);
  lightP[4].diffuse = onOff * glm::vec3(1.0, 1.0, 1.0);
  lightP[4].specular = onOff * glm::vec3(1.0, 1.0, 1.0);

  lightP[5].position = onOff * glm::vec3(10.5, 4.35, 25.5);
  lightP[5].ambient = onOff * glm::vec3(0.3, 0.3, 0.3);
  lightP[5].diffuse = onOff * glm::vec3(1.0, 1.0, 1.0);
  lightP[5].specular = onOff * glm::vec3(1.0, 1.0, 1.0);

  shaders.setLight("uLightG", lightG);
  for (int i = 0; i < NLD; i++)
    shaders.setLight("uLightD[" + toString(i) + "]", lightD[i]);

  for (int i = 0; i < NLP; i++)
    shaders.setLight("uLightP[" + toString(i) + "]", lightP[i]);

  for (int i = 0; i < 6; i++) {
    glm::mat4 M = glm::translate(I, lightP[i].position) * glm::scale(I, glm::vec3(0.025));
    drawObjectMat(sphere, mLuz, P, V, M);
  }

  for (int i = 0; i < NLF; i++)
    shaders.setLight("uLightF[" + toString(i) + "]", lightF[i]);

  // for (int i = 0; i < NLF; i++) {
  //   glm::mat4 M = glm::translate(I, lightF[i].position) * glm::scale(I, glm::vec3(0.025));
  //   drawObjectMat(sphere, mLuz, P, V, M);
  // }
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
  // shaders.setMat4("uN", glm::transpose(M));  Cambiando esto pasa de verse las luces de derecha a las de izquierda
  // creemos que el error de las luces tiene relacion con la normal
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
  glm::mat4 S = glm::scale(I, glm::vec3(0.5, 1.0, 0.5));
  glm::mat4 T = glm::translate(I, glm::vec3(-1.0, 2.23, -0.6));
  glm::mat4 R = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
  drawObjectMat(openCube, material, P, V, M * T * R * S);
  T = glm::translate(I, glm::vec3(-0.15, 2.23, -0.6));
  drawObjectMat(openCube, material, P, V, M * T * R * S);
  S = glm::scale(I, glm::vec3(0.94, 1.0, 0.065));
  T = glm::translate(I, glm::vec3(-0.58, 2.75, -0.6));
  drawObjectMat(cube, material, P, V, M * T * R * S);
}

void drawWheels(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
  glm::mat4 Rx = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
  glm::mat4 Tz = glm::translate(I, glm::vec3(0.45, 0.5, 0.3));
  glm::mat4 S = glm::scale(I, glm::vec3(0.2, 0.2, 0.2));

  glm::mat4 Rz = glm::rotate(I, glm::radians(rotCoches), glm::vec3(0, 0, 1));

  // Rueda trasera izquierda
  drawObjectMat(wheel, blackRubber, P, V, M * Tz * Rz * Rx * S);

  Tz = glm::translate(I, glm::vec3(-1.75, 0.5, 0.3));
  // Rueda delantera izquierda
  drawObjectMat(wheel, blackRubber, P, V, M * Tz * Rz * Rx * S);

  glm::mat4 Ry = glm::rotate(I, glm::radians(180.0f), glm::vec3(0, 1, 0));
  glm::mat4 Trs = glm::translate(I, glm::vec3(0.0, 0.0, -1.84));
  // Rueda delantera derecha
  drawObjectMat(wheel, blackRubber, P, V, M * Trs * Tz * Rz * Ry * Rx * S);

  glm::mat4 Tx = glm::translate(I, glm::vec3(0.45, 0.5, -1.3));
  // Rueda trasera derecha
  drawObjectMat(wheel, blackRubber, P, V, M * Tx * Rz * Rx * S);
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
  glm::mat4 S = glm::scale(I, glm::vec3(0.02, 0.87, 0.45));
  glm::mat4 T = glm::translate(I, glm::vec3(-1.5, 2.2, -0.6));
  glm::mat4 R = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
  glDepthMask(GL_FALSE);
  // Luna delantera
  drawObjectMat(cube, cyanPlastic, P, V, M * T * R * S);
  // Luna trasera
  T = glm::translate(I, glm::vec3(0.35, 2.2, -0.6));
  drawObjectMat(cube, cyanPlastic, P, V, M * T * R * S);
  // Ventanilla delantera izquierda
  S = glm::scale(I, glm::vec3(0.42, 0.02, 0.42));
  T = glm::translate(I, glm::vec3(-1.0, 2.255, 0.35));
  drawObjectMat(cube, cyanPlastic, P, V, M * T * R * S);
  // Ventanilla trasera izquierda
  T = glm::translate(I, glm::vec3(-0.15, 2.255, 0.35));
  drawObjectMat(cube, cyanPlastic, P, V, M * T * R * S);
  // Ventanilla delantera derecha
  T = glm::translate(I, glm::vec3(-1.0, 2.255, -1.55));
  drawObjectMat(cube, cyanPlastic, P, V, M * T * R * S);
  // Ventanilla trasera derecha
  T = glm::translate(I, glm::vec3(-0.15, 2.255, -1.55));
  drawObjectMat(cube, cyanPlastic, P, V, M * T * R * S);
  glDepthMask(GL_TRUE);
}

void drawCoche(glm::mat4 P, Material& material, glm::mat4 V, glm::mat4 M) {  // coches automaticos
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

void drawCocheD(glm::mat4 P, Material& material, glm::mat4 V, glm::mat4 M) {  // coche Dorado, el que nosotros movemos
  // Ruedas
  drawWheels2(P, V, M);
  // Chasis coche
  drawCube(P, material, V, M);
  drawCube2(P, material, V, M);
  // Luces
  drawLights(P, V, M);
  // Ventanillas
  drawWindows(P, V, M);
}

void drawWheels2(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
  glm::mat4 Rx = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
  glm::mat4 Ry2I = glm::rotate(I, glm::radians(ruedaIzquierda), glm::vec3(0, 1, 0));
  glm::mat4 Ry2D = glm::rotate(I, glm::radians(ruedaDerecha), glm::vec3(0, 1, 0));
  glm::mat4 Tz = glm::translate(I, glm::vec3(0.45, 0.5, 0.3));
  glm::mat4 S = glm::scale(I, glm::vec3(0.2, 0.2, 0.2));

  glm::mat4 Rz = glm::rotate(I, glm::radians(rotCocheDorado), glm::vec3(0, 0, 1));

  // Rueda trasera izquierda
  drawObjectMat(wheel, blackRubber, P, V, M * Tz * Rz * Rx * S);

  Tz = glm::translate(I, glm::vec3(-1.75, 0.5, 0.3));
  // Rueda delantera izquierda
  drawObjectMat(wheel, blackRubber, P, V, M * Tz * Ry2I * Rz * Rx * S);

  glm::mat4 Ry = glm::rotate(I, glm::radians(180.0f), glm::vec3(0, 1, 0));
  glm::mat4 Trs = glm::translate(I, glm::vec3(0.0, 0.0, -1.84));
  // Rueda delantera derecha
  drawObjectMat(wheel, blackRubber, P, V, M * Trs * Tz * Ry2I * Rz * Ry * Rx * S);

  glm::mat4 Tx = glm::translate(I, glm::vec3(0.45, 0.5, -1.3));
  // Rueda trasera derecha
  drawObjectMat(wheel, blackRubber, P, V, M * Tx * Rz * Rx * S);
}

void funFramebufferSize(GLFWwindow* window, int width, int height) {
  // Configuracion del Viewport
  glViewport(0, 0, width, height);

  // Actualizacion de w y h
  w = width;
  h = height;
}

void funKey(GLFWwindow* window, int key, int scancode, int action, int mods) {  // movimiento del coche Dorado
  // aqui movemos el coche, las ruedas para que roten al avanzar, y el giro de derecha e izquierda
  static bool wPressed = false;
  static bool aPressed = false;
  static bool sPressed = false;
  static bool dPressed = false;

  switch (action) {
    case GLFW_PRESS:
      switch (key) {
        case GLFW_KEY_F:
          onOff = (onOff == 1.0f) ? 0.0f : 1.0f;
          break;
        case GLFW_KEY_W:
          wPressed = true;
          break;
        case GLFW_KEY_A:
          aPressed = true;
          break;
        case GLFW_KEY_S:
          sPressed = true;
          break;
        case GLFW_KEY_D:
          dPressed = true;
          break;
      }
      break;
    case GLFW_RELEASE:
      switch (key) {
        case GLFW_KEY_W:
          wPressed = false;
          break;
        case GLFW_KEY_A:
          aPressed = false;
          break;
        case GLFW_KEY_S:
          sPressed = false;
          break;
        case GLFW_KEY_D:
          dPressed = false;
          break;
      }
      break;
  }

  if (wPressed && aPressed) {
    freno = 0.0;
    if (posDoradoX < 9.5) {
      rotCocheDorado += 10.0f;
      if (rotCocheDorado >= 360.0f) {
        rotCocheDorado = 0.0f;
      }
      ruedaDerecha = 45.0f;
      ruedaIzquierda = 45.0f;
      posDoradoX += 0.25 * glm::cos(glm::radians(-45.0f));
      posDoradoZ -= 0.25 * glm::sin(glm::radians(-45.0f));
      posLuzDoradoDelantera1X += 0.25 * glm::cos(glm::radians(-45.0f));
      posLuzDoradoDelantera1Z -= 0.25 * glm::sin(glm::radians(-45.0f));
      posLuzDoradoDelantera2X += 0.25 * glm::cos(glm::radians(-45.0f));
      posLuzDoradoDelantera2Z -= 0.25 * glm::sin(glm::radians(-45.0f));
      posLuzDoradoTrasera1X += 0.25 * glm::cos(glm::radians(-45.0f));
      posLuzDoradoTrasera1Z -= 0.25 * glm::sin(glm::radians(-45.0f));
      posLuzDoradoTrasera2X += 0.25 * glm::cos(glm::radians(-45.0f));
      posLuzDoradoTrasera2Z -= 0.25 * glm::sin(glm::radians(-45.0f));
    }
  } else if (wPressed && dPressed) {
    freno = 0.0;
    if (posDoradoX > 5.5) {
      rotCocheDorado += 10.0f;
      if (rotCocheDorado >= 360.0f) {
        rotCocheDorado = 0.0f;
      }
      ruedaDerecha = -45.0f;
      ruedaIzquierda = -45.0f;
      posDoradoX -= 0.25 * glm::cos(glm::radians(-45.0f));
      posDoradoZ -= 0.25 * glm::sin(glm::radians(-45.0f));
      posLuzDoradoDelantera1X -= 0.25 * glm::cos(glm::radians(-45.0f));
      posLuzDoradoDelantera1Z -= 0.25 * glm::sin(glm::radians(-45.0f));
      posLuzDoradoDelantera2X -= 0.25 * glm::cos(glm::radians(-45.0f));
      posLuzDoradoDelantera2Z -= 0.25 * glm::sin(glm::radians(-45.0f));
      posLuzDoradoTrasera1X -= 0.25 * glm::cos(glm::radians(-45.0f));
      posLuzDoradoTrasera1Z -= 0.25 * glm::sin(glm::radians(-45.0f));
      posLuzDoradoTrasera2X -= 0.25 * glm::cos(glm::radians(-45.0f));
      posLuzDoradoTrasera2Z -= 0.25 * glm::sin(glm::radians(-45.0f));
    }
  } else if (sPressed && aPressed) {
    freno = 1.0;
    if (posDoradoX < 9.5 && posDoradoZ > -2.8) {
      rotCocheDorado -= 10.0f;
      if (rotCocheDorado <= 0.0f) {
        rotCocheDorado = 360.0f;
      }
      ruedaDerecha = 45.0f;
      ruedaIzquierda = 45.0f;
      posDoradoX += 0.25 * glm::cos(glm::radians(-45.0f));
      posDoradoZ += 0.25 * glm::sin(glm::radians(-45.0f));
      posLuzDoradoDelantera1X += 0.25 * glm::cos(glm::radians(-45.0f));
      posLuzDoradoDelantera1Z += 0.25 * glm::sin(glm::radians(-45.0f));
      posLuzDoradoDelantera2X += 0.25 * glm::cos(glm::radians(-45.0f));
      posLuzDoradoDelantera2Z += 0.25 * glm::sin(glm::radians(-45.0f));
      posLuzDoradoTrasera1X += 0.25 * glm::cos(glm::radians(-45.0f));
      posLuzDoradoTrasera1Z += 0.25 * glm::sin(glm::radians(-45.0f));
      posLuzDoradoTrasera2X += 0.25 * glm::cos(glm::radians(-45.0f));
      posLuzDoradoTrasera2Z += 0.25 * glm::sin(glm::radians(-45.0f));
    }
  } else if (sPressed && dPressed) {
    freno = 1.0;
    if (posDoradoX > 5.5 && posDoradoZ > -2.8) {
      rotCocheDorado -= 10.0f;
      if (rotCocheDorado <= 0.0f) {
        rotCocheDorado = 360.0f;
      }
      ruedaDerecha = -45.0f;
      ruedaIzquierda = -45.0f;
      posDoradoX -= 0.25 * glm::cos(glm::radians(45.0f));
      posDoradoZ -= 0.25 * glm::sin(glm::radians(45.0f));
      posLuzDoradoDelantera1X -= 0.25 * glm::cos(glm::radians(-45.0f));
      posLuzDoradoDelantera1Z += 0.25 * glm::sin(glm::radians(-45.0f));
      posLuzDoradoDelantera2X -= 0.25 * glm::cos(glm::radians(-45.0f));
      posLuzDoradoDelantera2Z += 0.25 * glm::sin(glm::radians(-45.0f));
      posLuzDoradoTrasera1X -= 0.25 * glm::cos(glm::radians(-45.0f));
      posLuzDoradoTrasera1Z += 0.25 * glm::sin(glm::radians(-45.0f));
      posLuzDoradoTrasera2X -= 0.25 * glm::cos(glm::radians(-45.0f));
      posLuzDoradoTrasera2Z += 0.25 * glm::sin(glm::radians(-45.0f));
    }
  } else {
    if (wPressed) {
      freno = 0.0;
      rotCocheDorado += 10.0f;
      if (rotCocheDorado >= 360.0f) {
        rotCocheDorado = 0.0f;
      }
      ruedaDerecha = 0.0f;
      ruedaIzquierda = 0.0f;
      posDoradoZ += 0.25;
      posLuzDoradoDelantera1Z += 0.25;
      posLuzDoradoDelantera2Z += 0.25;
      posLuzDoradoTrasera1Z += 0.25;
      posLuzDoradoTrasera2Z += 0.25;
    }
    if (sPressed) {
      freno = 1.0;
      if (posDoradoZ > -2.8) {
        rotCocheDorado -= 10.0f;
        if (rotCocheDorado <= 0.0f) {
          rotCocheDorado = 360.0f;
        }
        ruedaDerecha = 0.0f;
        ruedaIzquierda = 0.0f;
        posDoradoZ -= 0.25;
        posLuzDoradoDelantera1Z -= 0.25;
        posLuzDoradoDelantera2Z -= 0.25;
        posLuzDoradoTrasera1Z -= 0.25;
        posLuzDoradoTrasera2Z -= 0.25;
      }
    }
    if (aPressed) {
      freno = 0.0;
      if (posDoradoX < 9.5) {
        ruedaIzquierda = 45.0f;
        ruedaDerecha = 45.0f;
        posDoradoX += 0.25;
        posLuzDoradoDelantera1X += 0.25;
        posLuzDoradoDelantera2X += 0.25;
        posLuzDoradoTrasera1X += 0.25;
        posLuzDoradoTrasera2X += 0.25;
      }
    }
    if (dPressed) {
      freno = 0.0;
      if (posDoradoX > 5.5) {
        ruedaIzquierda = -45.0f;
        ruedaDerecha = -45.0f;
        posDoradoX -= 0.25;
        posLuzDoradoDelantera1X -= 0.25;
        posLuzDoradoDelantera2X -= 0.25;
        posLuzDoradoTrasera1X -= 0.25;
        posLuzDoradoTrasera2X -= 0.25;
      }
    }
  }

  if (posDoradoX <= -27.0f) {
    // Volver a la posición inicial al llegar al final del mapa
    posDoradoX = 2.8;
    posDoradoZ = 6.2;
    posLuzDoradoDelantera1X = 5.4;
    posLuzDoradoDelantera1Z = -0.75;
    posLuzDoradoDelantera2X = 6.05;
    posLuzDoradoDelantera2Z = -0.75;
    posLuzDoradoTrasera1X = 5.4;
    posLuzDoradoTrasera1Z = -3.75;
    posLuzDoradoTrasera2X = 6.05;
    posLuzDoradoTrasera2Z = -3.75;
    rotCocheDorado = 0.0;
    ruedaDerecha = 0.0;
    ruedaIzquierda = 0.0;
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

void funTimer(double seconds, double& start) {
  if (glfwGetTime() - start > seconds /*  1/60  */) {
    posJade += 0.25;
    rotCoches += 5.0f;
    if (rotCoches >= 360.0) {
      rotCoches = 0.0;
    }

    if (posJade >= 22.0) {
      posJade = -9.0;
    }

    start = glfwGetTime();
  }
}

void funTimer2(double seconds, double& start2) {
  if (glfwGetTime() - start2 > seconds /*  1/60  */) {
    posGris += 0.2;
    rotCoches += 5.0f;
    if (rotCoches >= 360.0) {
      rotCoches = 0.0;
    }
    if (posGris >= 22.0) {
      posGris = -9.0;
    }
    start2 = glfwGetTime();
  }
}

void funTimer3(double seconds, double& start3) {
  if (glfwGetTime() - start3 > seconds /*  1/60  */) {
    posMarron -= 0.1;
    rotCoches += 5.0f;
    if (rotCoches >= 360.0) {
      rotCoches = 0.0;
    }
    if (posMarron <= -8.0) {
      posMarron = 23.0;
    }
    start3 = glfwGetTime();
  }
}

void funTimer4(double seconds, double& start4) {
  if (glfwGetTime() - start4 > seconds /*  1/60  */) {
    posVerde -= 0.15;
    rotCoches += 5.0f;
    if (rotCoches >= 360.0) {
      rotCoches = 0.0;
    }
    if (posVerde <= -8.0) {
      posVerde = 23.0;
    }
    start4 = glfwGetTime();
  }
}

// Luces coche Jade
void funTimer5(double seconds, double& start5) {
  if (glfwGetTime() - start5 > seconds /*  1/60  */) {
    posLuzJade += 0.25;
    rotCoches += 5.0f;
    if (rotCoches >= 360.0) {
      rotCoches = 0.0;
    }
    if (posLuzJade >= 24.0) {
      posLuzJade = -7.0;
    }
    start5 = glfwGetTime();
  }
}

// Coche verde del fondo
void funTimer6(double seconds, double& start6) {
  if (glfwGetTime() - start6 > seconds /*  1/60  */) {
    posLuzVerde += 0.20;
    rotCoches += 5.0f;
    if (rotCoches >= 360.0) {
      rotCoches = 0.0;
    }
    if (posLuzVerde >= 24.0) {
      posLuzVerde = -7.0;
    }
    start6 = glfwGetTime();
  }
}

// Luces coche Marron
void funTimer7(double seconds, double& start7) {
  if (glfwGetTime() - start7 > seconds /*  1/60  */) {
    posLuzMarron -= 0.10;
    rotCoches += 5.0f;
    if (rotCoches >= 360.0) {
      rotCoches = 0.0;
    }
    if (posLuzMarron <= -10.0) {
      posLuzMarron = 21.0;
    }
    start7 = glfwGetTime();
  }
}

// Luces coche Gris
void funTimer8(double seconds, double& start8) {
  if (glfwGetTime() - start8 > seconds /*  1/60  */) {
    posLuzGris -= 0.15;
    rotCoches += 5.0f;
    if (rotCoches >= 360.0) {
      rotCoches = 0.0;
    }
    if (posLuzGris <= -10.0) {
      posLuzGris = 21.0;
    }
    start8 = glfwGetTime();
  }
}
// Seria sacar el tamaño del coche con las posiciones x y z de un coche cualquiera y aplicarlo a todos, xyz del cubo
// grande del coche
//  Suponiendo que tienes la posición, ancho (width), altura (height) y profundidad (depth) del coche
double carWidth = 3.0;   // Ancho del coche
double carHeight = 1.5;  // Altura del coche
double carDepth = 4.0;   // Profundidad del coche (o largo)

// El tamaño del coche (o "bounding box") en un eje sería la mitad de su medida más grande
double carSize = fmax(carWidth, carDepth) / 2.0;
// este tamaño es el que enviamos siempre al size de checkCollision, ya que los coches son todos iguales de tamaño

bool checkCollision(double x1, double z1, double x2, double z2, double size) {
  // x1 z1 corresponde al dorado, x2 y z2 al otro coche que sea
  double distance = sqrt((x1 - x2) * (x1 - x2) + (z1 - z2) * (z1 - z2));
  return distance < size;
}