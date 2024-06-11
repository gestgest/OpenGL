#include "../../header/frame/FrameColor.h"

namespace frame 
{
	FrameColor::FrameColor()
	{
		color[0] = 0.2f;
		color[1] = 0.3f;
		color[2] = 0.8f;
		color[3] = 1.0f;
	}
	FrameColor::~FrameColor()
	{

	}
	
	void FrameColor::onUpdate(float deltatime)
	{

	}

	void FrameColor::onRender()
	{
		GLCHECK(glClearColor(color[0], color[1], color[2], color[3]));
		GLCHECK(glClear(GL_COLOR_BUFFER_BIT));
	}

	void FrameColor::onImGUIRender()
	{
		ImGui::ColorEdit4("clear color", color);
	}
}