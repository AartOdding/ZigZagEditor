#pragma once

#include <ZigZag/BaseOperator.hpp>

#include "WindowActions.hpp"
#include "CommandStack.hpp"
#include "gui/ApplicationStyle.hpp"


struct ApplicationState
{
	WindowActions windowActions;
	CommandStack commandStack;
	ApplicationStyle style;

	ZigZag::BaseOperator rootOperator{ nullptr, "project" };
};
