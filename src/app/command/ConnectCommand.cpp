#include "ConnectCommand.hpp"

#include <stdexcept>



ConnectDataCommand::ConnectDataCommand(ZigZag::BaseDataSource* source, ZigZag::BaseDataInput* input)
	: m_source(source),
	  m_input(input)
{
	if (!source || !input)
	{
		throw std::runtime_error("Cannot connect to null");
	}
	// TODO: throw if connection is not allowed
	m_initialSource = input->getConnectedOutput();
	m_description = "Connect " + source->getName() + " and " + input->getName();
}

bool ConnectDataCommand::redo()
{
	connect(m_source, m_input);
	return true;
}

bool ConnectDataCommand::undo()
{
	disconnect(m_source, m_input);

	if (m_initialSource)
	{
		connect(m_initialSource, m_input);
	}
	return true;
}

const std::string& ConnectDataCommand::getCommandName()
{
	static const std::string name = "ConnectDataCommand";
	return name;
}

const std::string& ConnectDataCommand::description()
{
	return m_description;
}



ConnectParametersCommand::ConnectParametersCommand(ZigZag::BaseParameter* source, ZigZag::BaseParameter* input)
	: m_source(source),
	  m_input(input)
{
	if (!source || !input)
	{
		throw std::runtime_error("Cannot connect to null");
	}
	// TODO: throw if connection is not allowed
	m_initialSource = input->getConnectedOutput();
	m_description = "Connect " + source->getName() + " and " + input->getName();
}

bool ConnectParametersCommand::redo()
{
	connect(m_source, m_input);
	return true;
}

bool ConnectParametersCommand::undo()
{
	disconnect(m_source, m_input);

	if (m_initialSource)
	{
		connect(m_initialSource, m_input);
	}
	return true;
}

const std::string& ConnectParametersCommand::getCommandName()
{
	static const std::string name = "ConnectParametersCommand";
	return name;
}

const std::string& ConnectParametersCommand::description()
{
	return m_description;
}
