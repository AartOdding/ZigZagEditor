#pragma once

#include <functional>
#include <string>
#include <string_view>

#include <imgui.h>



class Window
{
public:

	Window(std::string_view title, bool open = true);

	void update();

	void open();
	void close();

	bool isOpen() const;
	bool isDocked() const;
	bool hasFocus() const;

	const std::string& getTitle() const;

	void setDrawFunction(std::function<void()> drawFunction);

protected:

	virtual void draw();

private:

	std::string m_title;
	bool m_open = true;
	bool m_focus = false;
	bool m_docked = false;

	std::function<void()> m_drawFunction;
};
