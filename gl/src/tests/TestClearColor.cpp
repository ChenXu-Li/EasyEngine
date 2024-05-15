#include "TestClearColor.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
test::TestClearColor::TestClearColor()
	:m_ClearColor{1.0f,1.0f,0.0f,1.0f}
{
}

test::TestClearColor::~TestClearColor()
{
}

void test::TestClearColor::OnUpdate(float deltaTime)
{
}

void test::TestClearColor::OnRender()
{
	GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void test::TestClearColor::OnImGuiRender()
{
	ImGui::ColorEdit4("clear color", (float*)&m_ClearColor);
}
