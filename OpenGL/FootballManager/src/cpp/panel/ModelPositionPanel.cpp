#include "../../header/panel/ModelPositionPanel.h"


//opengl 강의에서의 TestTexture2D
namespace panel
{
	ModelPositionPanel::ModelPositionPanel()
	{
		for (int i = 0; i < MODEL_SIZE; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				trans_pos[i][j] = 0;
			}
		}

		//수학 그래프 기준 x,y축
		float pos[] = {
			-100.0f, -100.0f, 0.0f, 1.0f,
			100.0f, -100.0f, 0.0f, 0.0f,
			100.0f, 100.0f, 1.0f, 1.0f,
			-100.0f, 100.0f, 1.0f, 0.0f
		};

		unsigned int index_pos[6] = {
			0, 1, 2,
			1, 2, 3
		};

		//?
		GLCHECK(glEnable(GL_BLEND));
		GLCHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


		//무조건 포인터 계열은 정적(스택) 보다는 동적(힙) 생성을 해야한다. 그래야 이 함수를 나가도 유지가 된다
		va = new VertexArray();
		vb = new VertexBuffer(pos, sizeof(pos) * sizeof(float));
		ib = new IndexBuffer(index_pos, sizeof(index_pos));
		layout = new VertexBufferLayout();
		
		(*layout).push<float>(2);
		(*layout).push<float>(2);
		(*va).addBuffer(*vb, *layout);

		//행렬 카메라
		proj = glm::ortho(0.0f, 1200.0f, 0.0f, 900.0f, -1.0f, 1.0f); //처음 화면 자체가 4 : 3이다.
		view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0)); //카메라, 왼쪽으로 100 이동(translate)


		//쉐이더 생성
		sha = new Shader("./res/basic.shader");
		(*sha).bind();

		//텍스쳐 생성
		
		texture = new Texture("./res/texture/reactIcon.png");
		(*texture).bind();
		//(*sha).setUniform4f("u_color", 0.0f, 0.0f, 1.0f, 1.0f);
		(*sha).setUniform1i("u_texture", 0); //색 대신 텍스쳐를 넣은거


		//바인드된 버퍼를 해제
		(*va).unBind();
		(*vb).unBind();
		(*ib).unBind();
		sha->unBind();

		GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, 0)); //정점에 대한 데이터를 생성할 버퍼로 할당
		GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); //버퍼

	}
	ModelPositionPanel::~ModelPositionPanel()
	{
		delete(va);
		delete(vb);
		delete(ib);
		delete(layout);
		delete(texture);
		delete(sha); //delete 오류
	}

	void ModelPositionPanel::onUpdate(float deltatime)
	{

	}

	//메인에서의 while문
	void ModelPositionPanel::onRender()
	{
		GLCHECK(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCHECK(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		texture->bind();

		for (int i = 0; i < MODEL_SIZE; i++)
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), trans_pos[i]);
			glm::mat4 mvp = proj * view * model;
			sha->bind();
			sha->setUniformMat4f("u_mvp", mvp);
			renderer.draw(*va, *ib, *sha); //여러개의 배열에 draw함수는 여러번 호출해야 한다
		}
		//GLCHECK(glClearColor(color[0], color[1], color[2], color[3]));
		//GLCHECK(glClear(GL_COLOR_BUFFER_BIT));
	}

	void ModelPositionPanel::onImGUIRender()
	{
		for (int i = 0; i < MODEL_SIZE; i++)
		{
			//주소값으로 vector 3pos를 알아서 인식한다.
			ImGui::SliderFloat3(i + " - model : ", &trans_pos[i][0], 0.0f, 960.0f);
		}
		ImGui::Text("um");
	}
	
}