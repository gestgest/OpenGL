#pragma once

namespace frame 
{
	class Frame {
	public:
		Frame() {}
		virtual ~Frame() {}

		virtual void onUpdate(float deltatime) {}
		virtual void onRender() {}
		virtual void onImGUIRender() {}

	};
}