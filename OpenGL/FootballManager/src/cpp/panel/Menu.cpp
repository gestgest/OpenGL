#include "../../header/panel/Menu.h"


namespace panel
{
	Menu::Menu(Panel* current)
	{
		this->current = current;
	}
	Menu::~Menu()
	{

	}

	void Menu::onUpdate(float deltatime)
	{

	}

	void Menu::onRender()
	{
	}
	
	void Menu::onImGUIRender()
	{
		std::vector<std::pair<std::string, std::function<Panel* ()>>>::iterator it;
		for (it = items.begin(); items.end() != it; it++)
		{
			if (ImGui::Button(it->first.c_str()))
			{
				this->current = it->second();
			}
		}
	}

	template <typename T>
	void Menu::addItems(const std::string& name)
	{
		items.push_back(std::make_pair(name, []() { return new T(); }));
	}
}
