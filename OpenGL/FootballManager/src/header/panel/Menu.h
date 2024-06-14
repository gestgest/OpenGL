#pragma once
#include <vector>
#include <iterator>
#include <functional>

#include "../imgui/imgui.h"

#include "panel.h"
#include "../DebugRenderer.h"


namespace panel
{

	class Menu : public Panel {
	private:
		std::vector<std::pair<std::string, std::function<Panel* ()>>> items;
		Panel*& current;
	public:
		Menu(Panel*& current);
		~Menu();

		void onUpdate(float deltatime) override;
		void onRender() override;
		void onImGUIRender() override;

		template <typename T>
		void addItems(const std::string& name)
		{
			items.push_back(std::make_pair(name, []() { return new T(); }));
		}
	};

}