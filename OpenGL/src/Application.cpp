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
#include"Shader.h"



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


    Shader shader("res/shaders/Basic.shader");

    shader.Bind();

    shader.SetUniform4f("u_Color", 0.2f, 0.5f, 0.8f, 1.0f);

    /* Loop until the user closes the window */

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();
    float r = 0.0f;
    float increment = 0.05f;
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        /* Swap front and back buffers */
        va.Bind();
        ib.Bind();
        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.5f, 0.8f, 1.0f);
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        if (r >= 1.0f) increment = -0.05f;
        if (r <= 0.0f) increment = 0.05f;
        r += increment;
        GLCall(glfwSwapBuffers(window));
        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }
    GLCall(glfwTerminate());
    return 0;
}