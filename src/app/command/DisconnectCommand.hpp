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

	void redo() final;
	void undo() final;

	const std::string& typeName() final;
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

	void redo() final;
	void undo() final;

	const std::string& typeName() final;
	const std::string& description() final;

private:

	std::string m_description;

	ZigZag::BaseParameter* m_source{ nullptr };
	ZigZag::BaseParameter* m_input{ nullptr };

};
