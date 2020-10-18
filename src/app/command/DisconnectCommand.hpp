#pragma once

#include "Command.hpp"

#include <string>

#include <ZigZag/Object.hpp>
#include <ZigZag/BaseDataInput.hpp>
#include <ZigZag/BaseDataSource.hpp>
#include <ZigZag/BaseParameter.hpp>



class DisconnectDataCommand : public Command
{
public:

	DisconnectDataCommand(ZigZag::BaseDataSource* source, ZigZag::BaseDataInput* input);

	bool redo() final;
	bool undo() final;

	const std::string& getCommandName() final;
	const std::string& description() final;

private:

	std::string m_description;

	ZigZag::BaseDataSource* m_source{ nullptr };
	ZigZag::BaseDataInput* m_input{ nullptr };

};



class DisconnectParametersCommand : public Command
{
public:

	DisconnectParametersCommand(ZigZag::BaseParameter* source, ZigZag::BaseParameter* input);

	bool redo() final;
	bool undo() final;

	const std::string& getCommandName() final;
	const std::string& description() final;

private:

	std::string m_description;

	ZigZag::BaseParameter* m_source{ nullptr };
	ZigZag::BaseParameter* m_input{ nullptr };

};
