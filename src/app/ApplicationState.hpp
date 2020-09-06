#pragma once

#include <ZigZag/BaseOperator.hpp>

#include "command/CommandStack.hpp"
#include "app/style/ApplicationStyle.hpp"
#include "app/style/FontLibrary.hpp"


struct ApplicationState
{
	CommandStack commandStack;
	ApplicationStyle style;
	FontLibrary fontLibrary;

	ZigZag::BaseOperator rootOperator{ nullptr, "project" };
};
