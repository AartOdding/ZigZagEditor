#pragma once

#include <ZigZag/BaseOperator.hpp>

#include "WindowActions.hpp"
#include "CommandStack.hpp"


struct ApplicationState
{
	WindowActions windowActions;
	CommandStack commandStack;

	ZigZag::BaseOperator rootOperator{ nullptr, "project" };
};
