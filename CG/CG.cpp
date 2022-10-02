#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint createShaderProgram() {
    const char *vshaderSource =
        "#version 430 \n"
        "void main(void) \n"
        "{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";

    const char *fshaderSource = 
        "#version 430 \n"
        "out vec4 color; \n"
        "void main(void) \n"
        "{ color = vec4(0.0, 0.0, 1.0, 1.0); }";

    // Create a vertex shader
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    // Create a fragment shader
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Load GLSL code from string into the empty shader object
    glShaderSource(vShader, 1, &vshaderSource, NULL);
    glShaderSource(fShader, 1, &fshaderSource, NULL);

    // Compile each shader
    glCompileShader(vShader);
    glCompileShader(fShader);

    // Create an openGl program object
    GLuint vfProgram = glCreateProgram();
    // Attach the shader to openGl program object
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);

    return vfProgram;
}

using namespace std;

void init(GLFWwindow* window) {
    renderingProgram = createShaderProgram();
    /*!
        Even if the application doesn't use any buffers at all, 
        OpenGL still needs at least one created VAO when using shaders, 
        so these two lines are used to create the VAO required by OpenGL.
    */
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime) {
    //Load shaders into hardware
    glUseProgram(renderingProgram);
    glDrawArrays(GL_POINTS, 0, 1);
}

int main(void) {
    if (!glfwInit()) { exit(EXIT_FAILURE); }

    //Specify the openGl version number
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //Create the openGl context 
    GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter2 - program1", NULL, NULL);
    //Link the openGl context and glfwWindow
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }

    //Turn on vertical sync
    glfwSwapInterval(1);

    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
