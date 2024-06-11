#pragma once
#include "frame.h"
#include "../DebugRenderer.h"
#include "../imgui/imgui.h"

namespace frame
{
	class FrameColor : public Frame {
	private:
		float color[4];
	public:
		FrameColor();
		~FrameColor();
		void onUpdate(float deltatime) override;
		void onRender() override;
		void onImGUIRender() override;
	};
}
