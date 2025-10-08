//////////////////////////////////////////
//		jieunlee@hansung.ac.kr			//
//		Solar system (Earth)			//
//		2023. 5. 11						//
//////////////////////////////////////////
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#define STB_IMAGE_IMPLEMENTATION
//std_image.h를 이용해서 이미지 열려면 위에 이거 정의해야함
#include <std_image.h>

#include <../Hansung4/header/shader.h>
#include <../Hansung4/header/camera.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void init_sphere(float**, int*, int*);
void init_textures();

// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 50.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting => sun 0 0 0
glm::vec3 lightPos(21.0f,21.0f,11.0f);

// the sun, planets, and moon
	// 자전주기(일): 태양 25.38일, 수성 58.6462, 금성	-243.0185, 지구	0.99726963,	(달 27.3216) 화성은 1.02595676, 목성은 0.410일, 토성은 0.426일, 천왕성은 0.718일, 해왕성은 0.669일
	// 공전주기(일): 수성 87.97, 금성 224.7, 지구 365.26, (달 27.322) 화성 686.96, 목성 4333.29, 토성 10,756.20,	 천왕성 30,707.49, 	해왕성 60,223.35
	// rotation periods of the sun, planets, and moon
const float rotp_sun = 25.38f;
const float rotp_mercury = 58.6462f;
const float rotp_venus = 243.0185f;
const float rotp_earth = 0.99726963f;
const float rotp_moon = 27.3216f;
const float rotp_mars = 1.02595676f;
const float rotp_jupiter = 0.410f;
const float rotp_saturn = 0.426f;
const float rotp_uranus = 0.718f;
const float rotp_neptune = 0.669;

// revolution periods of the planets, and moon
const float revp_mercury = 87.97f;
const float revp_venus = 224.7f;
const float revp_earth = 365.26f;
const float revp_moon = 27.322f;
const float revp_mars = 686.96f;
const float revp_jupiter = 4333.29f;
const float revp_saturn = 10756.20f;
const float revp_uranus = 30707.49f;
const float revp_neptune = 60223.35;

// speed of rotation and revolution
const float rot_speed = 20.0f;

// scales of the planets, and moon 
const float radi_sun = 6.0f; // 696340.0f;
const float radi_mercury = 0.24f;	//2439.7f;
const float radi_venus = 0.60f;	//6051.8f;
const float radi_earth = 0.63f;	//6371.0f;
const float radi_moon = 0.17f;	//1737.4f;
const float radi_mars = 0.3389f;	//3389.5f;
const float radi_jupiter = 1.0f; // 69911.0f;
const float radi_saturn = 0.9f; // 58232.0f;
const float radi_uranus = 0.43f; // 25362.0f;
const float radi_neptune = 0.4f; // 24622.0f;

// textures
unsigned int texture_sun, texture_mercury, texture_venus, texture_earth, texture_moon;
unsigned int texture_mars, texture_jupiter, texture_saturn, texture_uranus, texture_neptune;


//생성 그리기
float setMatPlant(Shader & planetShader, glm::mat4 & basic_model, unsigned int & sphereVAO, int & nSphereVert, unsigned int texture, const float revp, const float radi, const float rotp, float dist)
{
	// world transformation
	dist = dist + radi_sun + 3 * radi; //태양 반지름 + 자기 반지름 3번
	glm::mat4 model = basic_model;

	//mercury를 다른 걸로 바꿔야함
	model = glm::rotate(model, (float)glfwGetTime() * rot_speed / revp, glm::vec3(0.0f, 1.0f, 0.0f));	// y축 공전 the revolution of the earth
	model = glm::translate(model, glm::vec3(dist, 0.0f, 0.0f));						// 
	model = glm::rotate(model, (float)glfwGetTime() * rot_speed / rotp, glm::vec3(0.0f, 1.0f, 0.0f));	// 자전 the rotation of the earth
	model = glm::scale(model, glm::vec3(radi, radi, radi)); //크기조정
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //북극을 위로
	planetShader.setMat4("model", model);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	// render the sphere
	glBindVertexArray(sphereVAO);
	glDrawArrays(GL_TRIANGLES, 0, nSphereVert);

	return dist;
}

int main()
{
	// glfw: initialize and configure
	glfwInit();

	//버전알려주기 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation => 윈도우 제목 바꿔라
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "2071375", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window안돼" << std::endl;
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
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram => tmi.별과 행성 vs는 같음
	Shader planetShader("src/vs/solarsystem_planet.vs", "src/fs/solarsystem_planet.fs");
	//Shader planetShader("solarsystem_color.vs", "solarsystem_color.fs");
	Shader starShader("src/vs/solarsystem_star.vs", "src/fs/solarsystem_star.fs"); //<- todo[ ] : src/fs/solarsystem_star.fs 수정해야함


	// sphere VAO and VBO
	//std::vector <float> data;
	float* sphereVerts = NULL;
	int nSphereVert, nSphereAttr;
	init_sphere(&sphereVerts, &nSphereVert, &nSphereAttr);

	unsigned int sphereVBO, sphereVAO;
	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(1, &sphereVBO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, nSphereVert * nSphereAttr * sizeof(float), sphereVerts, GL_STATIC_DRAW);

	glBindVertexArray(sphereVAO);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, nSphereAttr * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, nSphereAttr * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, nSphereAttr * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	free(sphereVerts);


	// init textures
	init_textures();


	// uncomment this call to draw in wireframe polygons.
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
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


		// sun 	
		// -----------
		// be sure to activate shader when setting uniforms/drawing objects
		starShader.use();
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		starShader.setMat4("projection", projection);
		starShader.setMat4("view", view);

		// eye position
		starShader.setVec3("eyePos", camera.Position);

		// draw the sphere object
		// light properties
		glm::vec3 lightColor(1.0, 1.0, 1.0);;
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.8f); // decrease the influence 
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influenc
		starShader.setVec3("light.ambient", ambientColor);
		starShader.setVec3("light.diffuse", diffuseColor);
		starShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		starShader.setVec3("light.position", lightPos);

		// material properties
		starShader.setVec3("material.ambient", 0.5f, 0.5f, 0.5f);
		starShader.setVec3("material.diffuse", 0.9f, 0.9f, 0.9f);
		starShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
		starShader.setFloat("material.shininess", 20.0f);

		
		glm::mat4 model = glm::mat4(1.0f);//단위행렬
		model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 basic_model = model;

		//sun?
		// world transformation
		//setMatPlant();
		//Shader & planetShader, glm::mat4 & basic_model, unsigned int & sphereVAO, int & nSphereVert, unsigned int texture, const float revp, const float radi, const float rotp
		model = glm::rotate(model, (float)glfwGetTime() * rot_speed / rotp_sun, glm::vec3(0.0f, 1.0f, 0.0f));	// the rotation of the sun
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(radi_sun, radi_sun, radi_sun));
		starShader.setMat4("model", model);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_sun);

		// render the sphere
		glBindVertexArray(sphereVAO);
		glDrawArrays(GL_TRIANGLES, 0, nSphereVert);





		// Planets, moon 	
		// -----------
		// be sure to activate shader when setting uniforms/drawing objects
		planetShader.use();
		// view/projection transformations
		planetShader.setMat4("projection", projection);
		planetShader.setMat4("view", view);
		// eye position
		planetShader.setVec3("eyePos", camera.Position);

		// draw the sphere object
		// light properties
		//glm::vec3 lightColor(1.0, 1.0, 1.0);;
		//glm::vec3 diffuseColor = lightColor * glm::vec3(0.8f); // decrease the influence 
		//glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influenc
		planetShader.setVec3("light.ambient", ambientColor);
		planetShader.setVec3("light.diffuse", diffuseColor);
		planetShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		planetShader.setVec3("light.position", lightPos);
		// material properties
		planetShader.setVec3("material.ambient", 0.5f, 0.5f, 0.5f);
		planetShader.setVec3("material.diffuse", 0.9f, 0.9f, 0.9f);
		planetShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
		planetShader.setFloat("material.shininess", 20.0f);

		float dist = 0;
		// mercury 	
		//Shader & planetShader, glm::mat4 & basic_model, unsigned int & sphereVAO, int & nSphereVert, unsigned int texture, const float revp, const float radi, const float rotp
		dist = setMatPlant(planetShader, basic_model, sphereVAO, nSphereVert, texture_mercury, revp_mercury, radi_mercury, rotp_mercury, dist);

		// venus 	
		dist += setMatPlant(planetShader, basic_model, sphereVAO, nSphereVert, texture_venus, revp_venus, radi_venus, rotp_venus, dist);


		// earth 	
		dist += setMatPlant(planetShader, basic_model, sphereVAO, nSphereVert, texture_earth, revp_earth, radi_earth, rotp_earth, dist);


		// moon 	 여기다가 적어야함
		// -----------
		//태양 기준으로 I와 시야각만 가져오고 나머지 버림
		//단 지구의 크기, 자전 제외하고는 다 가져와야함
		// + trans * 회전

		// mars 	
		dist += setMatPlant(planetShader, basic_model, sphereVAO, nSphereVert, texture_mars, revp_mars, radi_mars, rotp_mars, dist);

		// jupiter 	
		dist += setMatPlant(planetShader, basic_model, sphereVAO, nSphereVert, texture_jupiter, revp_jupiter, radi_jupiter, rotp_jupiter, dist);

		// saturn 	
		// -----------
		dist += setMatPlant(planetShader, basic_model, sphereVAO, nSphereVert, texture_saturn, revp_saturn, radi_saturn, rotp_saturn, dist);

		// uranus 	
		// -----------
		dist += setMatPlant(planetShader, basic_model, sphereVAO, nSphereVert, texture_uranus, revp_uranus, radi_uranus, rotp_uranus, dist);

		// neptune 	
		// -----------
		dist += setMatPlant(planetShader, basic_model, sphereVAO, nSphereVert, texture_neptune, revp_neptune, radi_neptune, rotp_neptune, dist);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &sphereVAO);
	glDeleteBuffers(1, &sphereVBO);

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

// initalize vertices of a sphere : position, normal, tex_coords. 
//void initSphere(std::vector <float> data, int* nVert, int* nAttr) => 매개변수 방정식?
void init_sphere(float** vertices, int* nVert, int* nAttr)
{
	//nAttr : 8
	// sphere: set up vertex data and configure vertex attributes
	float pi = acosf(-1.0f);	// pi = 3.14152... 
	float pi2 = 2.0f * pi;
	int nu = 40, nv = 20;
	const double du = pi2 / nu;
	const double dv = pi / nv;

	*nVert = (nv - 1) * nu * 6;		// two triangles
	*nAttr = 8;
	*vertices = (float*)malloc(sizeof(float) * (*nVert) * (*nAttr));

	float u, v;
	int k = 0;

	v = 0.0f;
	u = 0.0f;
	for (v = (-0.5f) * pi + dv; v < 0.5f * pi - dv; v += dv)
	{
		for (u = 0.0f; u < pi2; u += du)
		{
			// p(u,v)
			(*vertices)[k++] = cosf(v) * cosf(u); 	(*vertices)[k++] = cosf(v) * sinf(u);	(*vertices)[k++] = sinf(v); 	// position (x,y,z)
			(*vertices)[k++] = cosf(v) * cosf(u);	(*vertices)[k++] = cosf(v) * sinf(u);	(*vertices)[k++] = sinf(v);		// normal (x,y z)
			(*vertices)[k++] = u / pi2;				(*vertices)[k++] = (v + 0.5f * pi) / pi;	// texture coords (x t)
			// 이렇게 해서 8개의 속성 => nAttr


			// p(u+du,v)
			(*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du);	(*vertices)[k++] = sinf(v); 	// position
			(*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du);	(*vertices)[k++] = sinf(v);		// normal
			(*vertices)[k++] = (u + du) / pi2;			(*vertices)[k++] = (v + 0.5f * pi) / pi; // texture coords

			// p(u,v+dv)
			(*vertices)[k++] = cosf(v + dv) * cosf(u);	(*vertices)[k++] = cosf(v + dv) * sinf(u);	(*vertices)[k++] = sinf(v + dv);	// position
			(*vertices)[k++] = cosf(v + dv) * cosf(u);	(*vertices)[k++] = cosf(v + dv) * sinf(u);	(*vertices)[k++] = sinf(v + dv);	// normal
			(*vertices)[k++] = u / pi2;					(*vertices)[k++] = (v + dv + 0.5f * pi) / pi; // texture coords

			// p(u+du,v)
			(*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du);	(*vertices)[k++] = sinf(v); 	// position
			(*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du);	(*vertices)[k++] = sinf(v);		// normal
			(*vertices)[k++] = (u + du) / pi2;			(*vertices)[k++] = (v + 0.5f * pi) / pi; // texture coords

			// p(u+du,v+dv)
			(*vertices)[k++] = cosf(v + dv) * cosf(u + du);	(*vertices)[k++] = cosf(v + dv) * sinf(u + du);	(*vertices)[k++] = sinf(v + dv); 	// position
			(*vertices)[k++] = cosf(v + dv) * cosf(u + du);	(*vertices)[k++] = cosf(v + dv) * sinf(u + du);	(*vertices)[k++] = sinf(v + dv);	// normal
			(*vertices)[k++] = (u + du) / pi2;				(*vertices)[k++] = (v + dv + 0.5f * pi) / pi;  // texture coords

			// p(u,v+dv)
			(*vertices)[k++] = cosf(v + dv) * cosf(u);	(*vertices)[k++] = cosf(v + dv) * sinf(u);	(*vertices)[k++] = sinf(v + dv);	// position
			(*vertices)[k++] = cosf(v + dv) * cosf(u);	(*vertices)[k++] = cosf(v + dv) * sinf(u);	(*vertices)[k++] = sinf(v + dv);	// normal
			(*vertices)[k++] = u / pi2;					(*vertices)[k++] = (v + dv + 0.5f * pi) / pi; // texture coords
		}
	}
	// triangles around north pole and south pole
	for (u = 0.0f; u < pi2; u += du)
	{
		// triangles around north pole 
		// p(u,pi/2-dv)
		v = 0.5f * pi - dv;
		(*vertices)[k++] = cosf(v) * cosf(u); 	(*vertices)[k++] = cosf(v) * sinf(u);	(*vertices)[k++] = sinf(v); 	// position 
		(*vertices)[k++] = cosf(v) * cosf(u);	(*vertices)[k++] = cosf(v) * sinf(u);	(*vertices)[k++] = sinf(v);		// normal
		(*vertices)[k++] = u / pi2;				(*vertices)[k++] = (v + 0.5f * pi) / pi;	// texture coords

		// p(u+du,pi/2-dv)
		v = 0.5f * pi - dv;
		(*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du); (*vertices)[k++] = sinf(v); 	// position
		(*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du); (*vertices)[k++] = sinf(v);		// normal
		(*vertices)[k++] = (u + du) / pi2;			(*vertices)[k++] = (v + 0.5f * pi) / pi; // texture coords

		// p(u,pi/2) = (0, 1. 0)  ~ north pole
		v = 0.5f * pi;
		(*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du);	(*vertices)[k++] = sinf(v); 	 // position
		(*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du);	(*vertices)[k++] = sinf(v);		 // normal
		(*vertices)[k++] = (u + du) / pi2;			(*vertices)[k++] = 1.0f;  // texture coords

		// triangles around south pole
		// p(u,-pi/2) = (0, -1, 0)  ~ south pole
		v = (-0.5f) * pi;
		(*vertices)[k++] = cosf(v) * cosf(u); 	(*vertices)[k++] = cosf(v) * sinf(u);	(*vertices)[k++] = sinf(v); 		// position
		(*vertices)[k++] = cosf(v) * cosf(u);	(*vertices)[k++] = cosf(v) * sinf(u);	(*vertices)[k++] = sinf(v);			// normal
		(*vertices)[k++] = u / pi2;				(*vertices)[k++] = 0.0f; // texture coords

		// p(u+du,-pi/2+dv)
		v = (-0.5f) * pi + dv;
		(*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du); (*vertices)[k++] = sinf(v);	// position
		(*vertices)[k++] = cosf(v) * cosf(u + du);	(*vertices)[k++] = cosf(v) * sinf(u + du); (*vertices)[k++] = sinf(v);	// normal
		(*vertices)[k++] = (u + du) / pi2;				(*vertices)[k++] = (v + 0.5f * pi) / pi; // texture coords

		// p(u,-pi/2+dv)
		(*vertices)[k++] = cosf(v) * cosf(u);	(*vertices)[k++] = cosf(v) * sinf(u); (*vertices)[k++] = sinf(v);	// position
		(*vertices)[k++] = cosf(v) * cosf(u);	(*vertices)[k++] = cosf(v) * sinf(u); (*vertices)[k++] = sinf(v);	// normal
		(*vertices)[k++] = u / pi2;					(*vertices)[k++] = (v + 0.5f * pi) / pi; // texture coords
	}
}

void init_textures()
{
	// load and create a texture 
	// -------------------------
	int width, height, nrChannels;
	unsigned char* data;

	// texture_sun 
	// ---------
	glGenTextures(1, &texture_sun);
	glBindTexture(GL_TEXTURE_2D, texture_sun);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	//data = stbi_load("..\\textures\\solarsystem\\2k_sun.jpg", &width, &height, &nrChannels, 0);
	data = stbi_load("../Hansung4/textures/solarsystem/2k_sun.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture_earth 
	// ---------
	glGenTextures(1, &texture_earth);
	glBindTexture(GL_TEXTURE_2D, texture_earth);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	//data = stbi_load("..\\textures\\solarsystem\\2k_earth_daymap.jpg", &width, &height, &nrChannels, 0);
	data = stbi_load("../Hansung4/textures/solarsystem/2k_earth_daymap.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture_moon 
	// ---------
	glGenTextures(1, &texture_moon);
	glBindTexture(GL_TEXTURE_2D, texture_moon);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("../Hansung4/textures/solarsystem/2k_moon.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture_mercury 
	// ---------
	glGenTextures(1, &texture_mercury);
	glBindTexture(GL_TEXTURE_2D, texture_mercury);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("../Hansung4/textures/solarsystem/2k_mercury.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture_venus 
	// ---------
	glGenTextures(1, &texture_venus);
	glBindTexture(GL_TEXTURE_2D, texture_venus);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("../Hansung4/textures/solarsystem/2k_venus_surface.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture_mars 
	// ---------
	glGenTextures(1, &texture_mars);
	glBindTexture(GL_TEXTURE_2D, texture_mars);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("../Hansung4/textures/solarsystem/2k_mars.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture_jupiter 
	// ---------
	glGenTextures(1, &texture_jupiter);
	glBindTexture(GL_TEXTURE_2D, texture_jupiter);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("../Hansung4/textures/solarsystem/2k_jupiter.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture_saturn 
	// ---------
	glGenTextures(1, &texture_saturn);
	glBindTexture(GL_TEXTURE_2D, texture_saturn);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("../Hansung4/textures/solarsystem/2k_saturn.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture_uranus 
	// ---------
	glGenTextures(1, &texture_uranus);
	glBindTexture(GL_TEXTURE_2D, texture_uranus);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("../Hansung4/textures/solarsystem/2k_uranus.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture_neptune 
	// ---------
	glGenTextures(1, &texture_neptune);
	glBindTexture(GL_TEXTURE_2D, texture_neptune);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("../Hansung4/textures/solarsystem/2k_neptune.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}