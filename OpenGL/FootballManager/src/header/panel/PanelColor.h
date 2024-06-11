#pragma once
#include "panel.h"
#include "../DebugRenderer.h"
#include "../imgui/imgui.h"

namespace panel
{
	class PanelColor : public Panel {
	private:
		float color[4];
	public:
		PanelColor();
		~PanelColor();
		void onUpdate(float deltatime) override;
		void onRender() override;
		void onImGUIRender() override;
	};
}
