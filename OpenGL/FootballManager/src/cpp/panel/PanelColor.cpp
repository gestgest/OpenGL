#include "../../header/panel/PanelColor.h"


//opengl 강의에서의 TestTexture2D
namespace panel
{
	PanelColor::PanelColor()
	{
		//컬러들은 PanelColor 안에 있음

		//뭐 메인 소스들 다 여기에 넣던데?
		color[0] = 0.0f;
		color[1] = 0.0f;
		color[2] = 0.0f;
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