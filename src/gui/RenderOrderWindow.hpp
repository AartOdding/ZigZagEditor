#pragma once

#include <ZigZag/BaseOperator.hpp>
#include <ZigZag/RenderOrder.hpp>

#include "app/ApplicationState.hpp"


class RenderOrderWindow
{
public:

	RenderOrderWindow(std::string_view windowName, ApplicationState* appState);

	void setScope(ZigZag::BaseOperator* rootOperator);

	void draw(bool* p_open);


private:

	std::string m_windowName;

	ApplicationState* m_appState;

	ZigZag::Object* m_rootOperator;
	std::unique_ptr<ZigZag::RenderOrder> m_renderOrder;

};
