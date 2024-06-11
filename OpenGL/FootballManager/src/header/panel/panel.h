#pragma once

namespace panel 
{
	class Panel {
	public:
		Panel() {}
		virtual ~Panel() {}

		virtual void onUpdate(float deltatime) {} // 왜 = 0을 안쓰고 {}를 썼는가? => 재정의 안하고 싶은 함수는 안 적기 위해서
		virtual void onRender() {}
		virtual void onImGUIRender() {}

	};
}