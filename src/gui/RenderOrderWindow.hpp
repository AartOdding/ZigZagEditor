#pragma once

#include <ZigZag/BaseOperator.hpp>
#include <ZigZag/RenderOrder.hpp>

#include "app/ApplicationState.hpp"
#include "gui/Window.hpp"


class RenderOrderWindow : public Window
{
public:

	RenderOrderWindow(std::string_view windowName, ApplicationState* appState);

	void setScope(ZigZag::BaseOperator* rootOperator);

protected:

	void draw() override;


private:

	ApplicationState* m_appState;

	ZigZag::Object* m_rootOperator;
	std::unique_ptr<ZigZag::RenderOrder> m_renderOrder;

};
