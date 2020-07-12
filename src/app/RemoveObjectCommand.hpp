#pragma once

#include "Command.hpp"

#include <string>
#include <string_view>

#include <ZigZag/Object.hpp>



class RemoveObjectCommand : public Command
{

public:

	RemoveObjectCommand(ZigZag::Object* object);
	~RemoveObjectCommand();

	void redo() final;
	void undo() final;

	const std::string& typeName() final;
	const std::string& description() final;

private:

	ZigZag::Object* m_object{ nullptr };
	ZigZag::Object* m_parentObject{ nullptr };

	bool m_ownsObject{ false };

	std::string m_description;

};
