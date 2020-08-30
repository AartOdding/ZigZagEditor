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

void DisconnectDataCommand::redo()
{
	ZigZag::disconnect(m_source, m_input);
}

void DisconnectDataCommand::undo()
{
	ZigZag::connect(m_source, m_input);
}

const std::string& DisconnectDataCommand::typeName()
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

void DisconnectParametersCommand::redo()
{
	ZigZag::disconnect(m_source, m_input);
}

void DisconnectParametersCommand::undo()
{
	ZigZag::connect(m_source, m_input);
}

const std::string& DisconnectParametersCommand::typeName()
{
	static const std::string name = "DisconnectParametersCommand";
	return name;
}

const std::string& DisconnectParametersCommand::description()
{
	return m_description;
}
