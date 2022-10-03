#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#define numVAOs 1

void printShaderLog(GLuint shader) {
    int len = 0;
    int chWrittn = 0;
    char* log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*)malloc(len);
        glGetShaderInfoLog(shader, len, &chWrittn, log);
        std::cout << "Shader Info Log:" << log << std::endl;
        free(log);
    }
}

void printProgramLog(int prog) {
    int len = 0;
    int chWritten = 0;
    char* log;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        log = (char*)malloc(len);
        glGetProgramInfoLog(prog, len, &chWritten, log);
        std::cout << "Program Info Log:" << log << std::endl;
        free(log);
    }
}

bool checkOpenGLError() {
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
        std::cout << "glError: " << glErr << std::endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

string readShaderSource(const char* filePath) {
    string content;
    ifstream fileStream(filePath, ios::in);
    string line;

    if (!fileStream) {
        cout << "open file fail!" << endl;
    }

    while (!fileStream.eof()) {
        getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}

GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint createShaderProgram() {
    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;

    string vertShaderStr = readShaderSource("vertShader.glsl");
    string fragShaderStr = readShaderSource("fragShader.glsl");

    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();

    // Create a vertex shader
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    // Create a fragment shader
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Load GLSL code from string into the empty shader object
    glShaderSource(vShader, 1, &vertShaderSrc, NULL);
    glShaderSource(fShader, 1, &fragShaderSrc, NULL);

    // Compile each shader
    glCompileShader(vShader);
    checkOpenGLError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if (vertCompiled != 1) {
        std::cout << "vertex compliation failed" << std::endl;
        printShaderLog(vShader);
    }

    glCompileShader(fShader);
    checkOpenGLError();
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != 1) {
        std::cout << "vertex compliation failed" << std::endl;
        printShaderLog(fShader);
    }

    // Create an openGl program object
    GLuint vfProgram = glCreateProgram();
    // Attach the shader to openGl program object
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);

    glLinkProgram(vfProgram);
    checkOpenGLError();
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if (linked != 1) {
        std::cout << "linking failed" << std::endl;
        printProgramLog(vfProgram);
    }

    return vfProgram;
}

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
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(void) {
    if (!glfwInit()) { exit(EXIT_FAILURE); }

    //Specify the openGl version number
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    //Create the openGl context 
    GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter2 - program3 + 4", NULL, NULL);
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
