#include "../../header/panel/PanelColor.h"

namespace panel
{
	PanelColor::PanelColor()
	{
		color[0] = 0.2f;
		color[1] = 0.3f;
		color[2] = 0.8f;
		color[3] = 1.0f;
	}
	PanelColor::~PanelColor()
	{

	}
	
	void PanelColor::onUpdate(float deltatime)
	{

	}

	void PanelColor::onRender()
	{
		GLCHECK(glClearColor(color[0], color[1], color[2], color[3]));
		GLCHECK(glClear(GL_COLOR_BUFFER_BIT));
	}

	void PanelColor::onImGUIRender()
	{
		ImGui::ColorEdit4("clear color", color);
	}
}