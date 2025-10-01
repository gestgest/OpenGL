//#include <iostream>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//using namespace std;
//
////opengl은 이렇게 콜백 함수들을 이용한다
//
////리사이즈 콜백함수
//void frameBuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}
//
////키보드 입력
//void processInput(GLFWwindow * window)
//{
//    //esc
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//    {
//        glfwSetWindowShouldClose(window, true);
//    }
//}
//
//int main()
//{
//
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//
//    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    glViewport(0, 0, 800, 600);
//    glfwSetFramebufferSizeCallback(window, frameBuffer_size_callback); //윈도우 리사이즈 함수 콜백
//       
//    while (!glfwWindowShouldClose(window))
//    {
//        processInput(window);
//
//        //rendering
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        glfwSwapBuffers(window); //색상버퍼 교체 => 더블버퍼
//        glfwPollEvents(); //입력 이벤트 발생하는지
//    }
//
//    glfwTerminate(); //glfw 리소스 모두 제거
//    return 0;
//}