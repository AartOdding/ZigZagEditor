#include "gui/Window.hpp"

#include <iostream>

Window::Window(std::string_view title)
	: m_title(title)
{

}


void Window::update()
{
	if (isOpen())
	{
		auto colorId = hasFocus() ? ImGuiCol_TabActive : ImGuiCol_TabUnfocusedActive;
		auto targetId = isDocked() ? ImGuiCol_ChildBg : ImGuiCol_WindowBg;
		//ImGui::PushStyleColor(targetId, ImGui::GetStyleColorVec4(colorId));

		if (ImGui::Begin(m_title.c_str(), &m_open))
		{
			draw();
		}

		m_focus = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
		m_docked = ImGui::IsWindowDocked();

		ImGui::End();

		//ImGui::PopStyleColor(1);
	}
	else
	{
		m_focus = false;
		m_docked = false;
	}
}

void Window::draw()
{
	if (m_drawFunction)
	{
		m_drawFunction();
	}
}

void Window::setDrawFunction(std::function<void()> drawFunction)
{
	m_drawFunction = std::move(drawFunction);
}

void Window::open()
{
	m_open = true;
}


bool Window::isOpen() const
{
	return m_open;
}


bool Window::isDocked() const
{
	return m_docked;
}


bool Window::hasFocus() const
{
	return m_focus;
}


const std::string& Window::getTitle() const
{
	return m_title;
}
