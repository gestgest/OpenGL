#pragma once
#include "panel.h"
#include "../DebugRenderer.h"
#include "../imgui/imgui.h"


#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../Shader.h"
#include "../Texture.h"

#define MODEL_SIZE 2

namespace panel
{
	class ModelPositionPanel : public Panel {
	private:
		glm::mat4 proj;
		glm::mat4 view;

		glm::vec3 trans_pos[MODEL_SIZE];

		//정점 배열, 정점 버퍼, 인덱스 버퍼, 정점 버퍼 레이아웃 => 스마트 포인트를 써봐야 하나? Memory
		VertexArray *va;
		VertexBuffer *vb;
		IndexBuffer* ib;
		VertexBufferLayout* layout;

		Shader* sha;
		Texture* texture;
	public:
		ModelPositionPanel();
		~ModelPositionPanel();
		void onUpdate(float deltatime) override;
		void onRender() override;
		void onImGUIRender() override;
	};
}
