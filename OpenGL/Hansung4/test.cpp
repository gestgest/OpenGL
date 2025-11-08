#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <../Hansung4/header/shader.h>
#include <../Hansung4/header/camera.h>

#include <iostream>

using namespace std;

#define nParticles 300
#define nVertAttri 6


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(10.0f, 20.0f, 10.0f));
glm::vec3 lightPos(1.2f, 5.0f, 12.0f);


//mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    const char title_name[] = { 50,48,55,49,51,55,53,32,236,149,136,236,167,132,237,152,129,0 };
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title_name, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //glEnable(GL_DEPTH_TEST);
    Shader groundShader("src/vs/j13.human.vs", "src/fs/j13.human.fs");
    //Shader groundShader("src/vs/solarsystem_planet.vs", "src/fs/solarsystem_planet.fs");

    // ========================================
    //          땅(Ground) VAO/VBO 설정
    // ========================================
    float groundVertices[] = {
        // positions            // normals 
         25.0f, -5.0f,  25.0f,   0.0f, 1.0f, 0.0f,
        -25.0f, -5.0f,  25.0f,   0.0f, 1.0f, 0.0f,
        -25.0f, -5.0f, -25.0f,   0.0f, 1.0f, 0.0f,

         25.0f, -5.0f,  25.0f,   0.0f, 1.0f, 0.0f,
        -25.0f, -5.0f, -25.0f,   0.0f, 1.0f, 0.0f,
         25.0f, -5.0f, -25.0f,   0.0f, 1.0f, 0.0f
    };

    unsigned int groundVAO, groundVBO;
    glGenVertexArrays(1, &groundVAO);
    glGenBuffers(1, &groundVBO);

    glBindVertexArray(groundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

    // 1. 위치(Position) 속성 (layout (location = 0))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 2. 법선(Normal) 속성 (layout (location = 1))
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //셰이더 행렬 미리 적용
        groundShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(camera.Position, glm::vec3(0.0f, 0.0f, 0.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
        groundShader.setMat4("projection", projection);
        groundShader.setMat4("view", view);


        // draw the sphere object
        // light properties
        glm::vec3 lightColor(1.0, 1.0, 1.0);;
        groundShader.setVec3("lightColor", lightColor);
        groundShader.setVec3("lightPos", lightPos);
        groundShader.setVec3("viewPos", camera.Position);

        // input
        // -----
        processInput(window);

        // render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //배경색
        glClear(GL_COLOR_BUFFER_BIT);


        // --- 1. 땅 그리기 ---
        // 땅은 특별한 모델 변환이 필요 없으므로 기본 행렬 사용
        glm::mat4 groundModel = glm::mat4(1.0f);
        groundModel = glm::translate(groundModel, glm::vec3(0.0f, -0.5f, 0.0f)); // 땅을 약간 아래로
        groundShader.setMat4("model", groundModel);

        groundShader.setVec3("objectColor", glm::vec3(0.1f, 1.0f, 0.0f));

        //glBindVertexArray(groundVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6); //삼각형

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &groundVAO);
    glDeleteBuffers(1, &groundVBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    //camera.ProcessMouseMovement(xoffset, yoffset);
}
