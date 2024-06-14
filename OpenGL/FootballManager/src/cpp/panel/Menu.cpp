#include "../../header/panel/Menu.h"


namespace panel
{
	//*&로 해야지 current의 메모리가 실시간으로 적용된다
	Menu::Menu(Panel*& current) : current(current)
	{
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

}
