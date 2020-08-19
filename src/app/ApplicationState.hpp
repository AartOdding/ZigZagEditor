#pragma once

#include <ZigZag/BaseOperator.hpp>

#include "command/CommandStack.hpp"
#include "app/style/ApplicationStyle.hpp"


struct ApplicationState
{
	CommandStack commandStack;
	ApplicationStyle style;

	ZigZag::BaseOperator rootOperator{ nullptr, "project" };
};
