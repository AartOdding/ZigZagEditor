#include "DisconnectCommand.hpp"

#include <stdexcept>



DisconnectDataCommand::DisconnectDataCommand(ZigZag::BaseDataSource* source, ZigZag::BaseDataInput* input)
	: m_source(source),
	m_input(input)
{
	if (!source || !input)
	{
		throw std::runtime_error("Cannot disconnect from null");
	}
	// TODO: throw if connection is not allowed
	m_description = "Disconnect " + source->getName() + " and " + input->getName();
}

bool DisconnectDataCommand::redo()
{
	ZigZag::disconnect(m_source, m_input);
	return true;
}

bool DisconnectDataCommand::undo()
{
	ZigZag::connect(m_source, m_input);
	return true;
}

const std::string& DisconnectDataCommand::getCommandName()
{
	static const std::string name = "DisconnectDataCommand";
	return name;
}

const std::string& DisconnectDataCommand::description()
{
	return m_description;
}



DisconnectParametersCommand::DisconnectParametersCommand(ZigZag::BaseParameter* source, ZigZag::BaseParameter* input)
	: m_source(source),
	m_input(input)
{
	if (!source || !input)
	{
		throw std::runtime_error("Cannot disconnect from null");
	}
	// TODO: throw if connection is not allowed
	m_description = "Disconnect " + source->getName() + " and " + input->getName();
}

bool DisconnectParametersCommand::redo()
{
	ZigZag::disconnect(m_source, m_input);
	return true;
}

bool DisconnectParametersCommand::undo()
{
	ZigZag::connect(m_source, m_input);
	return true;
}

const std::string& DisconnectParametersCommand::getCommandName()
{
	static const std::string name = "DisconnectParametersCommand";
	return name;
}

const std::string& DisconnectParametersCommand::description()
{
	return m_description;
}
