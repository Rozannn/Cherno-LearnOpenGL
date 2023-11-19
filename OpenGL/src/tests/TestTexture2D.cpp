#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"
#include "VertexBufferLayout.h"
namespace test
{
	TestTexture2D::TestTexture2D()
		:m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 720.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f))),
        m_Translation(glm::vec3(480.0f, 360.0f, 0.0f))
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        float positions[] = {
            100.0f, 100.0f, 0.0f, 0.0f, // 0
            200.0f, 100.0f, 1.0f, 0.0f,  // 1
            200.0f, 200.0f, 1.0f, 1.0f,    // 2
            100.0f, 200.0f, 0.0f, 1.0f   // 3
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VertexArray = std::make_unique<VertexArray>();
        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);


        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");

        /* Loop until the user closes the window */
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
        m_Shader->SetUniform1i("u_Texture", 0);

        m_Texture = std::make_unique<Texture>("res/textures/ChernoLogo.png");
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{

	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Texture->Bind();

        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
        glm::mat4 mvp = m_Proj * m_View * model;

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", mvp);
        renderer.Clear();
        renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
	}

	void TestTexture2D::OnImGuiRender()
	{
        ImGui::SliderFloat3("m_TranslationA", &m_Translation.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
