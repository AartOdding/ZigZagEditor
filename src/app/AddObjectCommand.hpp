#pragma once

#include "Command.hpp"

#include <string>
#include <string_view>

#include <ZigZag/Object.hpp>



class AddObjectCommand : public Command
{

public:

	AddObjectCommand(ZigZag::Object* parentObject, const std::string& typeName);
	~AddObjectCommand();

	void redo() final;
	void undo() final;

	const std::string& typeName() final;
	const std::string& getDescription() final;

private:

	ZigZag::Object* m_parentObject{ nullptr };
	ZigZag::Object* m_addedObject{ nullptr };

	bool m_ownsObject{ false };

	std::string m_description;

};
