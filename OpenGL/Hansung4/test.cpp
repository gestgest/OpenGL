#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <../Hansung4/header/shader.h>
#include <../Hansung4/header/camera.h>

#include "header/teapot_loader.h"
#include <stdlib.h>"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// curve
glm::vec3 BezierCrvEval(float t);	// Cubic Bezier Curve

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "2071375", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader teapotShader1("src/vs/14.2.teapot.vs", "src/fs/14.2.teapot.fs");
	Shader curveShader("src/vs/j17.curve.vs", "src/fs/j17.curve.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	int crvVertNum = 100;
	float crvVertices[300];
	glm::vec3 crvVert;

	int j = 0;
	float t = 0.0f;

	//0 ~ 100
	for (int i = 0; i < crvVertNum; i++)
	{
		t = (float)i / (crvVertNum - 1);

		crvVert = BezierCrvEval(t);
		crvVertices[j++] = crvVert.x;
		crvVertices[j++] = crvVert.y;
		crvVertices[j++] = crvVert.z;
	}

	// curve VAO
	unsigned int curveVAO, curveVBO;
	glGenVertexArrays(1, &curveVAO);
	glGenBuffers(1, &curveVBO);
	glBindVertexArray(curveVAO);
	glBindBuffer(GL_ARRAY_BUFFER, curveVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(crvVertices), &crvVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// teapot VAO and VBO
	std::vector <float> data;
	Teapot teapot("models/teapot.vbo", data, 8);

	unsigned int teapotVBO, teapotVAO;
	glGenVertexArrays(1, &teapotVAO);
	glGenBuffers(1, &teapotVBO);
	glBindBuffer(GL_ARRAY_BUFFER, teapotVBO);
	glBufferData(GL_ARRAY_BUFFER, teapot.nVertNum * teapot.nVertFloats * sizeof(float), &data[0], GL_STATIC_DRAW);

	glBindVertexArray(teapotVAO);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, teapot.nVertFloats * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, teapot.nVertFloats * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, teapot.nVertFloats * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// render loop
	// -----------
	t = 0.0f;		// float dt = 0.01f;
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// be sure to activate shader when setting uniforms/drawing objects
		teapotShader1.use();
		// view/projection transformations
		//t += deltaTime;	// t += dt;
		//*
		//t += 0.1f;
		//glm::vec3 crvVert;
		//crvVert = glm::vec3(rad * cos(t * 2 * pi / crvVertNum), 0.0f, rad * sin(t * 2 * pi / crvVertNum));
		//camera.Reset(crvVert, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) - crvVert);
		//*/
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		teapotShader1.setMat4("projection", projection);
		teapotShader1.setMat4("view", view);
		// eye position
		teapotShader1.setVec3("eyePos", camera.Position);

		// draw the teapot object 
		// light properties
		glm::vec3 lightColor(1.0, 1.0, 1.0);;
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence 
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influenc
		teapotShader1.setVec3("light.ambient", ambientColor);
		teapotShader1.setVec3("light.diffuse", diffuseColor);
		teapotShader1.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		teapotShader1.setVec3("light.position", lightPos);
		// material properties
		teapotShader1.setVec3("material.ambient", 0.7f, 0.5f, 0.3f);
		teapotShader1.setVec3("material.diffuse", 0.7f, 0.5f, 0.3f);
		teapotShader1.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
		teapotShader1.setFloat("material.shininess", 30.0f);
		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		teapotShader1.setMat4("model", model);
		// render the teapot
		glLineWidth(1.0);
		glBindVertexArray(teapotVAO);
		glDrawArrays(GL_TRIANGLES, 0, teapot.nVertNum);

		// draw the curve object
		curveShader.use();
		curveShader.setMat4("projection", projection);
		curveShader.setMat4("view", view);
		curveShader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
		// world transformation
		model = glm::mat4(1.0f);
		curveShader.setMat4("model", model);
		glLineWidth(3.0);
		glBindVertexArray(curveVAO);
		glDrawArrays(GL_LINE_STRIP, 0, crvVertNum);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &teapotVAO);
	glDeleteVertexArrays(1, &curveVAO);
	glDeleteBuffers(1, &teapotVBO);
	glDeleteBuffers(1, &curveVBO);

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

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}


//3차 베지어 커브 식
glm::vec3 BezierCrvEval(float t)	// Cubic Bezier Curve
{
	glm::vec3 P[4] = { {-2.0f, 0.0f, 0.0f}, {-1.0f, 1.0f, 0.0f}, {2.0f, -2.0f, 0.0f}, {3.0f, 0.0f, 0.0f} };

	float t2 = t * t;
	float t3 = t * t * t;
	float s = 1.0f - t;
	float s2 = s * s;
	float s3 = s * s * s;
	glm::vec3 p = s3 * P[0] + 3.0f * t * s2 * P[1] + 3.0f * t2 * s * P[2] + t3 * P[3];
	//glm::vec3 p = (1.0f - t) * (1.0f - t) * (1.0f - t) * P[0] + 3.0f * t * (1.0f - t) * (1.0f - t) * P[1] + 3.0f * t * t * (1.0f - t) * P[2] + t * t * t * P[3];
	return p;
}
