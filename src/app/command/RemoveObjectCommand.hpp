#pragma once

#include "Command.hpp"

#include <string>
#include <utility>
#include <vector>

#include <ZigZag/Object.hpp>
#include <ZigZag/BaseDataInput.hpp>
#include <ZigZag/BaseDataSource.hpp>
#include <ZigZag/BaseParameter.hpp>



class RemoveObjectCommand : public Command
{

public:

	RemoveObjectCommand(ZigZag::Object* object);
	~RemoveObjectCommand();

	bool redo() final;
	bool undo() final;

	const std::string& getCommandName() final;
	const std::string& description() final;

private:

	void recursivelyDisconnect(ZigZag::Object* object);
	void restoreConnections();

	std::vector<std::pair<ZigZag::BaseDataSource*, ZigZag::BaseDataInput*>> m_dataDisconnections;
	std::vector<std::pair<ZigZag::BaseParameter*, ZigZag::BaseParameter*>> m_parameterDisconnections;
	
	std::string m_description;

	ZigZag::Object* m_object{ nullptr };
	ZigZag::Object* m_parentObject{ nullptr };

	bool m_ownsObject{ false };

};
