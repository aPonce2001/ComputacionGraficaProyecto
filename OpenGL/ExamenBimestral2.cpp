#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>

//Declaración de funciones
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

//Valores de la ventana. Resolución 1280x720
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

//Ejecución de la cámara
Camera camera(glm::vec3(136.0f, -0.5f, 220.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Cálculo de deltaTime para el procesador
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//lighting //Luz direccional (global)
glm::vec3 lightPos(450.0f, 450.0f, 450.0f);
//glm::vec3 colorlightPredominant(0.07f, 0.07f, 0.07f);
glm::vec3 colorlightPredominant(0.784f, 0.392f, 0.314f); //Ambiental simular luz de luna
glm::vec3 ufoPosition(0.0f, 125.0f, 100.0f);  //posicion del ufo
glm::vec3 ufoLightColor(0.38f,1.0f,1.0f);   //Luz de la linterna grande

int main()
{
	//Inicialización de la librería GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//Creación de la ventana
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Examen Bimestral - Grupo 4", NULL, NULL);
	if (window == NULL)
	{
	std::cout << "Failed to create GLFW window" << std::endl;
	glfwTerminate();
	return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	//Configuración de GLFW para la captura del mouse
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Carga de la librería GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
	std::cout << "Failed to initialize GLAD" << std::endl;
	return -1;
	}

	//Configurar stb_image.h para que las texturas se volteen verticalmente
	//stbi_set_flip_vertically_on_load(true);

	//Se habilita el buffer-Z
	glEnable(GL_DEPTH_TEST);

	//Creación de los shaders por medio de la clase Shader Class
	Shader lightingShader("shaders/shader.vs", "shaders/shader.fs");
	Shader lightPredominat("shaders/shader_lightcube.vs", "shaders/shader_lightcube.fs");

	//Carga de los modelos a usar
	Model sceneModel("model/generalscene/Dragon Rock.obj");
	Model bugModel("model/bug/bug.obj");
	Model ufoModel("model/ufo/ufo.obj");
	Model alienModel("model/alien/alien.obj");
	Model zombieModel("model/zombie/zombie.obj");
	Model monsterModel("model/monster/monster.obj");
	Model portalModel("model/portal/portal.obj");
	Model stoneModel("model/stone/stone.obj");
	Model bossModel("model/boss/boss.obj");
	Model dragonModel("model/dragon/dragon.obj");

	float vertices[] = {
		// positions	  // normals	   // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	glm::vec3 pointLightPositions[] = {
		//Luz central
		glm::vec3(-7.0f, 134.0f, 13.0f),
		//Luces periféricas
		glm::vec3(128.0f, 56.0f, 133.0f),
		glm::vec3(112.0f, 56.0f,-124.0f),
		glm::vec3(-190.0f, 56.0f,-110.0f),
		glm::vec3(-188.0f, 56.0f, 131.0f),
		//Luz portal
		glm::vec3(29.0f, 7.0f, -214.0f),
		//Luz stone
		glm::vec3(25.0f, 47.0f, -213.0f)
	};

	// first, configure the cube's VAO (and VBO)
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(cubeVAO);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Exercise 13 Task 2
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);
	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	//Descomentar para el dibujado en wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// shader configuration
	// --------------------
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);
	lightingShader.setInt("material.emission", 2);

	glm::vec3 pointLightColors[] = {
		//Luz central
		glm::vec3(1.0f, 1.0f, 0.4f),
		//Luces periféricas
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		//Luz portal
		glm::vec3(0.38f, 1.0f, 1.0f),
		//Luz stone
		glm::vec3(0.38f, 1.0f, 1.0f)
	};

	camera.MovementSpeed = 70.0f; //Optional. Modify the speed of the camera

	//Render loops
	while (!glfwWindowShouldClose(window))
	{
		//Lógica de tiempo por frames
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Detección del inputs
		processInput(window);

		//Renderización de la ventana
		glClearColor(0.004f, 0.07f, 0.145f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		lightingShader.use();

		//Directional Light
		//lightingShader.setVec3("light.position", lightPos);
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 36.0f);

		lightingShader.setVec3("dirLight.direction", -1.0f, -1.0f, -1.0f);
		lightingShader.setVec3("dirLight.ambient", 0.07f, 0.07f, 0.07f);
		lightingShader.setVec3("dirLight.diffuse", colorlightPredominant);;
		//ourShader.setVec3("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		lightingShader.setVec3("dirLight.specular", glm::vec3(0.9f, 0.9f, 0.9f));

		// point light 1
		lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightingShader.setVec3("pointLights[0].ambient", pointLightColors[0].x * 0.08, pointLightColors[0].y * 0.08, pointLightColors[0].z * 0.08);
		lightingShader.setVec3("pointLights[0].diffuse", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		lightingShader.setVec3("pointLights[0].specular", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
		lightingShader.setFloat("pointLights[0].constant", 0.25f);
		lightingShader.setFloat("pointLights[0].linear", 0.009);
		lightingShader.setFloat("pointLights[0].quadratic", 0.0032);
		// point light 2
		lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightingShader.setVec3("pointLights[1].ambient", pointLightColors[1].x * 0.08, pointLightColors[1].y * 0.08, pointLightColors[1].z * 0.08);
		lightingShader.setVec3("pointLights[1].diffuse", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
		lightingShader.setVec3("pointLights[1].specular", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
		lightingShader.setFloat("pointLights[0].constant", 0.25f);
		lightingShader.setFloat("pointLights[0].linear", 0.009);
		lightingShader.setFloat("pointLights[0].quadratic", 0.0032);
		// point light 3
		lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		lightingShader.setVec3("pointLights[2].ambient", pointLightColors[2].x * 0.08, pointLightColors[2].y * 0.08, pointLightColors[2].z * 0.08);
		lightingShader.setVec3("pointLights[2].diffuse", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
		lightingShader.setVec3("pointLights[2].specular", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
		lightingShader.setFloat("pointLights[0].constant", 0.25f);
		lightingShader.setFloat("pointLights[0].linear", 0.009);
		lightingShader.setFloat("pointLights[0].quadratic", 0.0032);
		// point light 4
		lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		lightingShader.setVec3("pointLights[3].ambient", pointLightColors[3].x * 0.08, pointLightColors[3].y * 0.08, pointLightColors[3].z * 0.08);
		lightingShader.setVec3("pointLights[3].diffuse", pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
		lightingShader.setVec3("pointLights[3].specular", pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
		lightingShader.setFloat("pointLights[0].constant", 0.25f);
		lightingShader.setFloat("pointLights[0].linear", 0.009);
		lightingShader.setFloat("pointLights[0].quadratic", 0.0032);
		// point light 5
		lightingShader.setVec3("pointLights[4].position", pointLightPositions[4]);
		lightingShader.setVec3("pointLights[4].ambient", pointLightColors[4].x * 0.08 , pointLightColors[4].y * 0.08, pointLightColors[4].z * 0.08);
		lightingShader.setVec3("pointLights[4].diffuse", pointLightColors[4].x, pointLightColors[4].y, pointLightColors[4].z);
		lightingShader.setVec3("pointLights[4].specular", pointLightColors[4].x, pointLightColors[4].y, pointLightColors[4].z);
		lightingShader.setFloat("pointLights[0].constant", 0.25f);
		lightingShader.setFloat("pointLights[0].linear", 0.009);
		lightingShader.setFloat("pointLights[0].quadratic", 0.0032);
		//point light 6
		lightingShader.setVec3("pointLights[5].position", pointLightPositions[5]);
		lightingShader.setVec3("pointLights[5].ambient", pointLightColors[5].x * 0.08, pointLightColors[5].y * 0.08, pointLightColors[5].z * 0.08);
		lightingShader.setVec3("pointLights[5].diffuse", pointLightColors[5].x, pointLightColors[5].y, pointLightColors[5].z);
		lightingShader.setVec3("pointLights[5].specular", pointLightColors[5].x, pointLightColors[5].y, pointLightColors[5].z);
		lightingShader.setFloat("pointLights[0].constant", 0.25f);
		lightingShader.setFloat("pointLights[0].linear", 0.009);
		lightingShader.setFloat("pointLights[0].quadratic", 0.0032);
		//point light 7
		lightingShader.setVec3("pointLights[6].position", pointLightPositions[6]);
		lightingShader.setVec3("pointLights[6].ambient", pointLightColors[6].x * 0.08, pointLightColors[6].y * 0.08, pointLightColors[6].z * 0.08);
		lightingShader.setVec3("pointLights[6].diffuse", pointLightColors[6].x, pointLightColors[6].y, pointLightColors[6].z);
		lightingShader.setVec3("pointLights[6].specular", pointLightColors[6].x, pointLightColors[6].y, pointLightColors[6].z);
		lightingShader.setFloat("pointLights[0].constant", 0.25f);
		lightingShader.setFloat("pointLights[0].linear", 0.009);
		lightingShader.setFloat("pointLights[0].quadratic", 0.0032);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);


		// DIBUJO DEL UFO
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, diffuseMapStone);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, specularMapStone);

		float angulo = currentFrame * 14;
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		lightingShader.setMat4("model", model);
		sceneModel.Draw(lightingShader);
		
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, emissionMapStone);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-36.0f + 5.0f * sin(2 * currentFrame), 10.0f + sin(currentFrame), 180.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		bugModel.Draw(lightingShader);
		//glBindTexture(GL_TEXTURE_2D, 0);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(35.0f, 39.0f, -230.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		lightingShader.setMat4("model", model);
		bossModel.Draw(lightingShader);
		
		// spotLight
		lightingShader.setVec3("spotLight.direction", 0.0f, -1.0f, 0.0f);
		lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(32.5f)));
		lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(35.5f)));
		lightingShader.setVec3("spotLight.ambient", 0.01f, 0.01f, 0.01f);
		lightingShader.setFloat("spotLight.constant", 0.0003f);
		lightingShader.setFloat("spotLight.linear", 0.00009f);
		lightingShader.setFloat("spotLight.quadratic", 0.000032f);
		lightingShader.setVec3("spotLight.position", ufoPosition.x, ufoPosition.y + sin(currentFrame) * 1.7f, ufoPosition.z);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(ufoPosition.x, ufoPosition.y + sin(currentFrame) * 1.7f, ufoPosition.z)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		//model = glm::translate(model, glm::vec3(currentFrame*10, 10.0f, currentFrame*10)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		ufoModel.Draw(lightingShader);

		/*model = glm::translate(model, glm::vec3(ufoPosition.x, ufoPosition.y + sin(currentFrame) * 1.7f, ufoPosition.z)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));// it's a bit too big for our scene, so scale it down
		//model = glm::translate(model, glm::vec3(currentFrame*10, 10.0f, currentFrame*10)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		pigsModel.Draw(lightingShader);*/
		if(camera.Position.x > ufoPosition.x - 65.0f && camera.Position.x < ufoPosition.x + 65.0f && camera.Position.z > ufoPosition.z - 65.0f && camera.Position.z < ufoPosition.z + 65.0f){
			lightingShader.setVec3("spotLight.diffuse", 1.0f, 0.0f, 0.0f);
			lightingShader.setVec3("spotLight.specular", 1.0f, 0.0f, 0.0f);
			if(camera.Position.y < ufoPosition.y){
				camera.Position.y += 0.35f;
			}
			if(camera.Position.y < ufoPosition.y + 2.0f){
				camera.Position.x += (ufoPosition.x - camera.Position.x)/50.0f;
				camera.Position.z += (ufoPosition.z - camera.Position.z)/50.0f;
			}
		}else{
			lightingShader.setVec3("spotLight.diffuse", ufoLightColor);
			lightingShader.setVec3("spotLight.specular", ufoLightColor);
		}
		ufoPosition.x = 175.0f * sin(0.1f * currentFrame);
		ufoPosition.z = 100.0f * cos(0.1f * currentFrame);
		
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, emissionMapStone);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(25.0f, 47.0f + sin(currentFrame) * 0.5, -213.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		stoneModel.Draw(lightingShader);
		


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.0f, 3.2f, -154.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));	
		lightingShader.setMat4("model", model);
		zombieModel.Draw(lightingShader);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(46.0f, 3.2f, -154.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));	
		lightingShader.setMat4("model", model);
		zombieModel.Draw(lightingShader);

		//Junta de aliens
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(14.0f, 39.0f, -220.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(currentFrame*10, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		lightingShader.setMat4("model", model);
		alienModel.Draw(lightingShader);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(23.0f, 39.0f, -226.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		alienModel.Draw(lightingShader);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(42.0f, 39.0f, -218.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(currentFrame*10, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		lightingShader.setMat4("model", model);
		alienModel.Draw(lightingShader);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(30.0f, 39.0f, -199.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		alienModel.Draw(lightingShader);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-176.0f, 0.0f, -88.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
		lightingShader.setMat4("model", model);
		monsterModel.Draw(lightingShader);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(60.0f, -8.0f, 25.0f)); // translate it down so it's at the center of the scene	
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
		model = glm::rotate(model, glm::radians(165.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		dragonModel.Draw(lightingShader);

		//Alien oculto
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(155.0f, 4.0f, 242.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(currentFrame*10, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		lightingShader.setMat4("model", model);
		alienModel.Draw(lightingShader);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(29.0f, 7.0f, -214.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down
		lightingShader.setMat4("model", model);
		portalModel.Draw(lightingShader);

		// also draw the lamp object
		lightPredominat.use();
		lightPredominat.setVec3("lightColorCube", colorlightPredominant);
		lightPredominat.setMat4("projection", projection);
		lightPredominat.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(10.0f)); // a smaller cube
		lightPredominat.setMat4("model", model);
		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//Point Lights

		lightPredominat.use();
		glBindVertexArray(lightCubeVAO);
		for (unsigned int i = 0; i < 5; i++)
		{
			lightPredominat.setVec3("lightColorCube", pointLightColors[i]);
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(1.0f)); // Make it a smaller cube
			lightPredominat.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		lightPredominat.setVec3("lightColorCube", colorlightPredominant);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(100.0f));
		lightPredominat.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//Cambio de buffers frente a inputs u outputs
		glfwSwapBuffers(window);
		glfwPollEvents();
		}

		//Se termina la librería GLFW
		glfwTerminate();
		return 0;
}

//Función que procesa los input, en especial la tecla Esc, que cierra la ventana.
//También se agregan las teclas WASD para el movimiento libre de la cámara
void processInput(GLFWwindow *window)
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

//Función de GLFW que detecta los cambios de resolución de la ventana para el ajuste de los elementos
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//Llama al mouse a la ventana
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

//Llama al scroll del mouse a la ventana
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
