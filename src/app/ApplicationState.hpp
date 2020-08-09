#pragma once

#include <ZigZag/BaseOperator.hpp>

#include "WindowActions.hpp"
#include "command/CommandStack.hpp"
#include "app/style/ApplicationStyle.hpp"


struct ApplicationState
{
	WindowActions windowActions;
	CommandStack commandStack;
	ApplicationStyle style;

	ZigZag::BaseOperator rootOperator{ nullptr, "project" };
};
