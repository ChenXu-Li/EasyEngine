#include "Test.h"
#include"imgui/imgui.h"
namespace test {
	Testmenu::Testmenu(Test*& currentTestPointer)
		:m_CurrentTest(currentTestPointer)
	{
	}

	void Testmenu::OnImGuiRender()
	{
		for (auto& test : m_Tests) {
			if (ImGui::Button(test.first.c_str())) {
				m_CurrentTest = test.second();
			}
		}
	}
}

