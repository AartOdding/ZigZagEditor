#pragma once

#include <string>
#include <string_view>

#include <imgui.h>



class Window
{
public:

	Window(std::string_view title);

	void update();

	void open();

	bool isOpen() const;
	bool isDocked() const;
	bool hasFocus() const;

	const std::string& getTitle() const;

protected:

	virtual void draw() = 0;

private:

	std::string m_title;
	bool m_open = true;
	bool m_focus = false;
	bool m_docked = false;

};
