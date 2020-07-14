#pragma once

#include "Command.hpp"

#include <string>

#include <ZigZag/Object.hpp>
#include <ZigZag/BaseDataInput.hpp>
#include <ZigZag/BaseDataSource.hpp>
#include <ZigZag/BaseParameter.hpp>



class ConnectDataCommand : public Command
{
public:

	ConnectDataCommand(ZigZag::BaseDataSource* source, ZigZag::BaseDataInput* input);

	void redo() final;
	void undo() final;

	const std::string& typeName() final;
	const std::string& description() final;

private:

	std::string m_description;

	ZigZag::BaseDataSource* m_source{ nullptr };
	ZigZag::BaseDataInput* m_input{ nullptr };
	ZigZag::BaseDataSource* m_initialSource{ nullptr };

};



class ConnectParametersCommand : public Command
{
public:

	ConnectParametersCommand(ZigZag::BaseParameter* source, ZigZag::BaseParameter* input);

	void redo() final;
	void undo() final;

	const std::string& typeName() final;
	const std::string& description() final;

private:

	std::string m_description;

	ZigZag::BaseParameter* m_source{ nullptr };
	ZigZag::BaseParameter* m_input{ nullptr };
	ZigZag::BaseParameter* m_initialSource{ nullptr };

};
