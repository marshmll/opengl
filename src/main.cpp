#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

void framebufferSizeCallback(GLFWwindow *window, const int width, const int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW Window." << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return 2;
    }

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // clang-format off
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    // clang-format on

    const char *fragment_shader_code = "#version 330 core\n"
                                       "out vec4 fragment_color;\n"
                                       "void main()\n"
                                       "{\n"
                                       "    fragment_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                       "}\n\0";

    const char *vertex_shader_code = "#version 330 core\n"
                                     "layout (location = 0) in vec3 a_pos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "  gl_Position = vec4(a_pos.x, a_pos.y, a_pos.z, 1.0);\n"
                                     "}\n\0";
    // Vertex Shader
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
    glCompileShader(vertex_shader);

    int success;
    char info_log[512];

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::cerr << "Failed to compile vertex shader: " << info_log << std::endl;
        glfwTerminate();
    }

    // Fragment Shader
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::cerr << "Failed to compile fragment shader: " << info_log << std::endl;
        glfwTerminate();
    }

    // Shader Program
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::cerr << "Failed to link shader program: " << info_log << std::endl;
        glfwTerminate();
    }

    glUseProgram(shader_program);

    // Shaders are not needed after linking.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    unsigned int VAO, VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        // Input.
        processInput(window);

        glClearColor(.4f, .4f, .8f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Drawing
        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Event Polling and buffer swap.
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
