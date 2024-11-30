#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"

void configScene();
void renderScene();
void drawObject(Model &model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSuelo (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAspa  (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawHelice(glm::mat4 P, glm::mat4 V, glm::mat4 M);

void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey            (GLFWwindow* window, int key  , int scancode, int action, int mods);

// Shaders
   Shaders shaders;

// Modelos
   Model plane;
   Model triangle;
   Model cone;  //JOSE MANUEL GARCIA LLAMAS 03169171R

// Viewport
   int w = 500;
   int h = 500;

// Animaciones
   float desY = 0.0;
   float rotY = 0.0;

int main() {

 // Inicializamos GLFW
    if(!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

 // Creamos la ventana
    GLFWwindow* window;
    window = glfwCreateWindow(w, h, "Sesion 5", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

 // Inicializamos GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;

 // Configuramos los CallBacks
    glfwSetFramebufferSizeCallback(window, funFramebufferSize);
    glfwSetKeyCallback(window, funKey);

 // Entramos en el bucle de renderizado
    configScene();
    while(!glfwWindowShouldClose(window)) {
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
    glPolygonOffset(1.0,1.0);

 // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

 // Modelos
    plane.initModel("resources/models/plane.obj");
    triangle.initModel("resources/models/triangle.obj");
    cone.initModel("resources/models/cone.obj");  //JOSE MANUEL GARCIA LLAMAS 03169171R
}

void renderScene() {

 // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 // Indicamos los shaders a utilizar
    shaders.useShaders();

 // Matriz P
    float fovy   = 60.0;
    float nplane =  0.1;
    float fplane = 25.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

 // Matriz V
    glm::vec3 eye   (0.0, 7.0, 10.0);  //JOSE MANUEL GARCIA LLAMAS 03169171R
    glm::vec3 center(0.0, 0.0, 0.0);
    glm::vec3 up    (0.0, 1.0,  0.0);
    glm::mat4 V = glm::lookAt(eye, center, up);

 // Dibujamos la escena
    drawSuelo(P,V,I);
    //JOSE MANUEL GARCIA LLAMAS 03169171R (Todo lo inferior del metodo)
    glm::mat4 T = glm::translate(I, glm::vec3(-2.5, desY, -2.5));
    glm::mat4 R = glm::rotate   (I, glm::radians(rotY), glm::vec3(0, 1, 0));
    drawHelice(P,V,T*R);

    glm::mat4 T2 = glm::translate(I, glm::vec3(-2.5, desY, 2.5));
    glm::mat4 R2 = glm::rotate   (I, glm::radians(rotY), glm::vec3(0, 1, 0));
    drawHelice(P,V,T2*R2);

    glm::mat4 T3 = glm::translate(I, glm::vec3(2.5, desY, 2.5));
    glm::mat4 R3 = glm::rotate   (I, glm::radians(rotY), glm::vec3(0, 1, 0));
    drawHelice(P,V,T3*R3);

    glm::mat4 T4 = glm::translate(I, glm::vec3(2.5, desY, -2.5));
    glm::mat4 R4 = glm::rotate   (I, glm::radians(rotY), glm::vec3(0, 1, 0));
    drawHelice(P,V,T4 * R4);

}

void drawObject(Model &model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uPVM",P*V*M);

    glEnable(GL_POLYGON_OFFSET_FILL);
        shaders.setVec3("uColor",color);
        model.renderModel(GL_FILL);
    glDisable(GL_POLYGON_OFFSET_FILL);

    shaders.setVec3("uColor",glm::vec3(1.0, 1.0, 1.0));
    model.renderModel(GL_LINE);

}

void drawSuelo(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(5.0, 1.0, 5.0)); 
    glm::mat4 R = glm::rotate  (I, glm::radians(90.0f), glm::vec3(0, 1, 0)); 
    drawObject(plane, glm::vec3(0.0, 0.0, 1.0), P, V, M*S*R);

}

void drawAspa(glm::mat4 P, glm::mat4 V, glm::mat4 M) { //JOSE MANUEL GARCIA LLAMAS 03169171R

   /*  ESTA SERIA SU VERSION CON EL TRIANGULO
    glm::mat4 S = glm::scale(I, glm::vec3(0.5, 1, 2.0));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 R = glm::rotate  (I, glm::radians(90.0f), glm::vec3(0, 0, 1));
    drawObject(triangle, glm::vec3(1.0, 0.0, 0.0), P, V, M*R*T*S);
   */

    glm::mat4 S = glm::scale(I, glm::vec3(0.5, 1, 0.25));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 R = glm::rotate  (I, glm::radians(90.0f), glm::vec3(0, 0, 1));
    drawObject(cone, glm::vec3(1.0, 0.0, 0.0), P, V, M*R*T*S);
}

void drawHelice(glm::mat4 P, glm::mat4 V, glm::mat4 M) {//JOSE MANUEL GARCIA LLAMAS 03169171R

    glm::mat4 Rz45 = glm::rotate(I, glm::radians(45.0f), glm::vec3(0, 1, 0));
    drawAspa(P,V,M);
    drawAspa(P,V,M*Rz45);
    drawAspa(P,V,M*Rz45*Rz45);
    drawAspa(P,V,M*Rz45*Rz45*Rz45);
    drawAspa(P,V,M*Rz45*Rz45*Rz45*Rz45);
    drawAspa(P,V,M*Rz45*Rz45*Rz45*Rz45*Rz45);
    drawAspa(P,V,M*Rz45*Rz45*Rz45*Rz45*Rz45*Rz45);
    drawAspa(P,V,M*Rz45*Rz45*Rz45*Rz45*Rz45*Rz45*Rz45);
    drawAspa(P,V,M*Rz45*Rz45*Rz45*Rz45*Rz45*Rz45*Rz45*Rz45);


}

void funFramebufferSize(GLFWwindow* window, int width, int height) {

 // Configuracion del Viewport
    glViewport(0, 0, width, height);

 // Actualizacion de w y h
    w = width;
    h = height;

}

void funKey(GLFWwindow* window, int key  , int scancode, int action, int mods) {

    switch(key) {
        case GLFW_KEY_DOWN:    desY -= 0.1; break;  //JOSE MANUEL GARCIA LLAMAS 03169171R
        case GLFW_KEY_UP:  desY += 0.1; break;  //JOSE MANUEL GARCIA LLAMAS 03169171R
        case GLFW_KEY_LEFT:  rotY += 5.0; break;  //JOSE MANUEL GARCIA LLAMAS 03169171R
        case GLFW_KEY_RIGHT: rotY -= 5.0; break;  //JOSE MANUEL GARCIA LLAMAS 03169171R
        default:
            desY = 0.0;
            rotY = 0.0;
    }

}