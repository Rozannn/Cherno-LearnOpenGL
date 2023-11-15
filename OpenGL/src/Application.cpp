#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include<string>
#include<sstream>
#include"Renderer.h"
#include"VertexBuffer.h"
#include"IndexBuffer.h"
#include"VertexArray.h"
#include"VertexBufferLayout.h"

struct ShaderSources 
{
    std::string vertexShader;
    std::string fragmentShader;
};

static ShaderSources parseShader(const std::string& filepath) 
{
    enum class shaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    shaderType type = shaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos) 
        {
            if (line.find("vertex") != std::string::npos)
                type = shaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = shaderType::FRAGMENT;
        }
        else 
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(const std::string& source, unsigned int type) 
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) 
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);

        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);


    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW Initial Error!" << std::endl;
    }
    /* Make the window's context current */

    float positions[] = {
       -0.5f,  -0.5f,
       0.5f,  -0.5f,
       0.5f,  0.5f,
       -0.5f, 0.5f
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    VertexBuffer vb(positions, 6 * 2 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    VertexArray va;
    va.AddBuffer(vb, layout);
    IndexBuffer ib(indices, 6);


    ShaderSources source = parseShader("res/shaders/Basic.shader");

    unsigned int shader = CreateShader(source.vertexShader, source.fragmentShader);
    glUseProgram(shader);

    int location = glGetUniformLocation(shader, "u_Color");
    glUniform4f(location, 0.2f, 0.5f, 0.8f, 1.0f);
    /* Loop until the user closes the window */

    va.Unbind();
    vb.Unbind();
    ib.Unbind();

    float r = 0.0f;
    float increment = 0.05f;
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        /* Swap front and back buffers */
        va.Bind();
        ib.Bind();
        glUniform4f(location, r, 0.5f, 0.8f, 1.0f);
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        if (r >= 1.0f) increment = -0.05f;
        if (r <= 0.0f) increment = 0.05f;
        r += increment;
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
    GLCall(glDeleteProgram(shader));
    GLCall(glfwTerminate());
    return 0;
}