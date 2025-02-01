#pragma once

namespace panel 
{
	class Panel {
	public:
		Panel() {}
		virtual ~Panel() {}


		// 왜 = 0을 안쓰고 {}를 썼는가? => 재정의 안하고 싶은 함수는 안 적기 위해서
		virtual void onUpdate(float deltatime) {}  //프레임마다
		virtual void onRender() {} //랜더링
		virtual void onImGUIRender() {} //IMGUI 버튼 그리는 리스트
	};
}