#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "src/header/Renderer.h"
#include "src/header/DebugRenderer.h"
#include "src/header/Shader.h"
#include "src/header/Texture.h"

#include "src/header/IndexBuffer.h"
#include "src/header/VertexBuffer.h"
#include "src/header/VertexArray.h"
#include "src/header/VertexBufferLayout.h"

#include "src/header/glm/glm.hpp"
#include "src/header/glm/gtc/matrix_transform.hpp"

#include "src/header/imgui/imgui.h"
#include "src/header/imgui/imgui_impl_glfw.h"
#include "src/header/imgui/imgui_impl_opengl3.h"

#include "src/header/panel/PanelColor.h"
#include "src/header/panel/Menu.h"

#define MODEL_SIZE 2


void error_callback(int error, const char* description)
{
    std::cerr << "Error: " << description << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main()
{
    const char* glsl_version = "#version 130";
    GLFWwindow* window;

    // GLFW 라이브러리 초기화
    if (!glfwInit())
        exit(EXIT_FAILURE);

    //3.3.0 버전
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 윈도우 생성
    window = glfwCreateWindow(900, 600, "My Title", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // 윈도우 이벤트 콜백 등록
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);

    // OpenGL 컨텍스트 생성
    glfwMakeContextCurrent(window);

    //수직 동기화 => 주사율에 맞는 화면전환
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "error" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << glGetString(GL_VERSION) << std::endl; // 버전확인


    GLCHECK(glEnable(GL_BLEND));
    GLCHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


    Renderer renderer;


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    panel::Panel* current_panel = NULL;
    panel::Menu* menu = new panel::Menu(current_panel);

    current_panel = menu;
    panel::PanelColor panelColor; //파괴하고 다른 Frame으로 바꿔야지 에러가 안 생김
    menu->addItems<panel::PanelColor>("name");


    // 렌더링 루프
    while (!glfwWindowShouldClose(window))
    {
        renderer.clear();

        panelColor.onUpdate(0.0f);
        panelColor.onRender();
        
        ImGui_ImplOpenGL3_NewFrame();

        if (current_panel)
        {
            current_panel->onUpdate(0.0f);
            current_panel->onRender();
            //버튼을 누르거나 menu가 아닌 경우
            if (current_panel != menu && ImGui::Button("<-")) 
            {
                delete current_panel;
                current_panel = menu;
            }
            ImGui::Begin("Frame");
            current_panel->onImGUIRender(); //GUI 그리는 함수
            ImGui::End();

        }

        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        panelColor.onImGUIRender();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // 종료
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}


/*

    ///////////////////////////////////////////////////////////////변수

    float dx = 512.0f, dy = 445.0f;
    glm::vec3 trans_pos[2];
    for(int i = 0; i < MODEL_SIZE; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            trans_pos[i][j] = 0;
        }
    }

    //수학 그래프 기준 x,y축
    float pos[] = {
        100.0f, 100.0f + dy, 0.0f, 1.0f,
        100.0f, 100.0f, 0.0f, 0.0f,
        100.0f + dx, 100.0f + dy, 1.0f, 1.0f,
        100.0f + dx, 100.0f, 1.0f, 0.0f
    };

    unsigned int index_pos[6] = {
        0, 1, 2,
        1, 2, 3
    };


    glm::mat4 proj = glm::ortho(0.0f, 1200.0f, 0.0f, 900.0f, -1.0f, 1.0f); //처음 화면 자체가 4 : 3이다.
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0)); //카메라, 왼쪽으로 100 이동(translate)

    VertexArray va;
    VertexBuffer vb(pos, sizeof(pos) * sizeof(float));
    IndexBuffer ib(index_pos, sizeof(index_pos));


    //VertexBufferLayout 라인
    VertexBufferLayout layout;
    layout.push<float>(2);
    layout.push<float>(2);
    va.addBuffer(vb, layout);

    float g = 0;
    float plus = 0.05f;


    //버퍼 메모리 복원
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    //쉐이더 생성
    Shader sha("./res/basic.shader");
    sha.bind();

    Texture texture("./res/texture/reactIcon.png");
    texture.bind();
    sha.setUniform1i("u_texture", 0);

    //바인드된 버퍼를 해제한다면?
    va.unBind();
    sha.unBind();
    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, 0)); //정점에 대한 데이터를 생성할 버퍼로 할당
    GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); //버퍼

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    //update

        sha.bind();

        for (int i = 0; i < MODEL_SIZE; i++)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), trans_pos[i]);
            glm::mat4 mvp = proj * view * model;
            sha.bind();
            sha.setUniformMat4f("u_mvp", mvp);
            renderer.draw(va, ib, sha); //여러개의 배열에 draw함수는 여러번 호출해야 한다
        }

        sha.setUniform4f("u_color", 0.0f, g, 1.0f, 1.0f);

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //GLCHECK(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

if (g < 0.0f) {
    plus = 0.05f;
}
else if (1.0f < g) {
    plus = -0.05f;
}
g += plus;

// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    static int counter = 0;

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    for (int i = 0; i < MODEL_SIZE; i++)
    {
        ImGui::SliderFloat3("trans_pos : " + i, &trans_pos[i][0], 0.0f, 1000.0f);            // Edit 1 float using a slider from 0.0f to 1000.0f
    }
    //SliderFloat3 => 텍스트, 시작 주소, min, max)
    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
    
*/