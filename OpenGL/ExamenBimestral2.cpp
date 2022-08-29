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

unsigned int loadTexture(const char* path);

//Valores de la ventana. Resolución 1280x720
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

//Ejecución de la cámara
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Cálculo de deltaTime para el procesador
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//lighting //Luz direccional (global)
glm::vec3 lightPos(50.0f, 200.0f, 20.0f);
glm::vec3 colorlightPredominant(0.03f, 0.03f, 0.03f);

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Exercise 16 Task 1", NULL, NULL);
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
    Shader lightingShader("shaders/shader_exercise16_mloading.vs", "shaders/shader_exercise16_mloading.fs");
    Shader lightPredominat("shaders/shader_lightcube.vs", "shaders/shader_lightcube.fs");

    //Carga de los modelos a usar
    //Model ourModel("model/backpack/backpack.obj");
    Model sceneModel("model/generalscene/BallroomBlitz.obj");
    Model stoneModel("model/stone/stone.obj");
    Model ufoModel("model/ufo/ufo_abduction_pigs.obj");
    Model alienModel("model/alien/alien.obj");
    Model zombieModel("model/zombie/zombie.obj");
    Model monsterModel("model/monster/monster.obj");
    Model portalModel("model/portal/portal.obj");

    float vertices[] = {
        // positions          // normals           // texture coords
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
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(-50.0f, 0.0f, 0.0f),
        glm::vec3(50.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 50.0f),
        glm::vec3(0.0f, 0.0f, -50.0f),
        //Posicion de luces para portal
        glm::vec3(5.0f, 3.0f, 15.0f),
        //Posicion de luces al rededor de la stone
        glm::vec3(6.0f, 9.04f, 15.0f),
        glm::vec3(4.0f, 9.08, 15.0f),
        glm::vec3(5.0f, 7.06f, 16.0f),
        glm::vec3(5.0f, 7.04f, 14.0f),
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
    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    unsigned int textureMapBumm = loadTexture("model/stone/Scene_-_Root_normal.png");
    unsigned int diffuseMapStone = loadTexture("model/stone/Scene_-_Root_baseColor.jpeg");
    //unsigned int specularMapStone = loadTexture("model/stone/Scene_-_Root_normal.png");
    unsigned int emissionMapStone = loadTexture("model/stone/Scene_-_Root_emissive.jpeg");
    cout << emissionMapStone << endl;

    // shader configuration
    // --------------------
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);
    lightingShader.setInt("material.emission", 2);

    glm::vec3 pointLightColors[] = {
        //
        glm::vec3(0.38f, 1.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f), //R G B
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.4f, 0.3f, 1.0f),
        glm::vec3(0.38f, 1.0f, 1.0f),
        glm::vec3(0.945f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.788f, 0.054f),
        glm::vec3(1.0f, 0.098f, 0.1f),
        glm::vec3(0.9f, 0.2f, 0.1f),
    };

    camera.MovementSpeed = 15; //Optional. Modify the speed of the camera

    //Render loop
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
        lightingShader.setVec3("light.position", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 36.0f);

        lightingShader.setVec3("dirLight.direction", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("dirLight.ambient", colorlightPredominant);
        lightingShader.setVec3("dirLight.diffuse", glm::vec3(0.24725f, 0.1995f, 0.0745f));
        //ourShader.setVec3("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setVec3("dirLight.specular", glm::vec3(0.9f, 0.9f, 0.9f));

        // point light 1
        lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        lightingShader.setVec3("pointLights[0].ambient", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
        lightingShader.setVec3("pointLights[0].diffuse", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
        lightingShader.setVec3("pointLights[0].specular", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
        lightingShader.setFloat("pointLights[0].constant", 1.0f);
        lightingShader.setFloat("pointLights[0].linear", 0.09);
        lightingShader.setFloat("pointLights[0].quadratic", 0.032);
        // point light 2
        lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        lightingShader.setVec3("pointLights[1].ambient", pointLightColors[1].x * 0.08, pointLightColors[1].y * 0.08, pointLightColors[1].z * 0.08);
        lightingShader.setVec3("pointLights[1].diffuse", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
        lightingShader.setVec3("pointLights[1].specular", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
        lightingShader.setFloat("pointLights[1].constant", 1.0f);
        lightingShader.setFloat("pointLights[1].linear", 0.09);
        lightingShader.setFloat("pointLights[1].quadratic", 0.032);
        // point light 3
        lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        lightingShader.setVec3("pointLights[2].ambient", pointLightColors[2].x * 0.08, pointLightColors[2].y * 0.08, pointLightColors[2].z * 0.08);
        lightingShader.setVec3("pointLights[2].diffuse", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
        lightingShader.setVec3("pointLights[2].specular", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
        lightingShader.setFloat("pointLights[2].constant", 1.0f);
        lightingShader.setFloat("pointLights[2].linear", 0.09);
        lightingShader.setFloat("pointLights[2].quadratic", 0.032);
        // point light 4
        lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        lightingShader.setVec3("pointLights[3].ambient", pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
        lightingShader.setVec3("pointLights[3].diffuse", pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
        lightingShader.setVec3("pointLights[3].specular", pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
        lightingShader.setFloat("pointLights[3].constant", 1.0f);
        lightingShader.setFloat("pointLights[3].linear", 0.09);
        lightingShader.setFloat("pointLights[3].quadratic", 0.032);
        // point light 5
        lightingShader.setVec3("pointLights[4].position", pointLightPositions[4]);
        lightingShader.setVec3("pointLights[4].ambient", pointLightColors[4].x, pointLightColors[4].y, pointLightColors[4].z);
        lightingShader.setVec3("pointLights[4].diffuse", pointLightColors[4].x, pointLightColors[4].y, pointLightColors[4].z);
        lightingShader.setVec3("pointLights[4].specular", pointLightColors[4].x, pointLightColors[4].y, pointLightColors[4].z);
        lightingShader.setFloat("pointLights[4].constant", 1.0f);
        lightingShader.setFloat("pointLights[4].linear", 0.09);
        lightingShader.setFloat("pointLights[4].quadratic", 0.032);
        //point light 6
        lightingShader.setVec3("pointLights[5].position", pointLightPositions[5]);
        lightingShader.setVec3("pointLights[5].ambient", pointLightColors[5].x * 0.1, pointLightColors[5].y * 0.1, pointLightColors[5].z * 0.1);
        lightingShader.setVec3("pointLights[5].diffuse", pointLightColors[5].x, pointLightColors[5].y, pointLightColors[5].z);
        lightingShader.setVec3("pointLights[5].specular", pointLightColors[5].x, pointLightColors[5].y, pointLightColors[5].z);
        lightingShader.setFloat("pointLights[5].constant", 1.0f);
        lightingShader.setFloat("pointLights[5].linear", 0.04);
        lightingShader.setFloat("pointLights[5].quadratic", 0.009);
        //point light 7
        lightingShader.setVec3("pointLights[6].position", pointLightPositions[6]);
        lightingShader.setVec3("pointLights[6].ambient", pointLightColors[6].x * 0.1, pointLightColors[6].y * 0.1, pointLightColors[6].z * 0.1);
        lightingShader.setVec3("pointLights[6].diffuse", pointLightColors[6].x, pointLightColors[6].y, pointLightColors[6].z);
        lightingShader.setVec3("pointLights[6].specular", pointLightColors[6].x, pointLightColors[6].y, pointLightColors[6].z);
        lightingShader.setFloat("pointLights[6].constant", 1.0f);
        lightingShader.setFloat("pointLights[6].linear", 0.7);
        lightingShader.setFloat("pointLights[6].quadratic", 0.5);
        //point light 8
        lightingShader.setVec3("pointLights[7].position", pointLightPositions[7]);
        lightingShader.setVec3("pointLights[7].ambient", pointLightColors[7].x * 0.1, pointLightColors[7].y * 0.1, pointLightColors[7].z * 0.1);
        lightingShader.setVec3("pointLights[7].diffuse", pointLightColors[7].x, pointLightColors[7].y, pointLightColors[7].z);
        lightingShader.setVec3("pointLights[7].specular", pointLightColors[7].x, pointLightColors[7].y, pointLightColors[7].z);
        lightingShader.setFloat("pointLights[7].constant", 1.0f);
        lightingShader.setFloat("pointLights[7].linear", 0.7);
        lightingShader.setFloat("pointLights[7].quadratic", 0.5);
        //point light 9
        lightingShader.setVec3("pointLights[8].position", pointLightPositions[8]);
        lightingShader.setVec3("pointLights[8].ambient", pointLightColors[8].x * 0.1, pointLightColors[8].y * 0.1, pointLightColors[8].z * 0.1);
        lightingShader.setVec3("pointLights[8].diffuse", pointLightColors[8].x, pointLightColors[8].y, pointLightColors[8].z);
        lightingShader.setVec3("pointLights[8].specular", pointLightColors[8].x, pointLightColors[8].y, pointLightColors[8].z);
        lightingShader.setFloat("pointLights[8].constant", 1.0f);
        lightingShader.setFloat("pointLights[8].linear", 0.7);
        lightingShader.setFloat("pointLights[8].quadratic", 0.5);
        //point light 10
        lightingShader.setVec3("pointLights[9].position", pointLightPositions[9]);
        lightingShader.setVec3("pointLights[9].ambient", pointLightColors[9].x * 0.1, pointLightColors[9].y * 0.1, pointLightColors[9].z * 0.1);
        lightingShader.setVec3("pointLights[9].diffuse", pointLightColors[9].x, pointLightColors[9].y, pointLightColors[9].z);
        lightingShader.setVec3("pointLights[9].specular", pointLightColors[9].x, pointLightColors[9].y, pointLightColors[9].z);
        lightingShader.setFloat("pointLights[9].constant", 1.0f);
        lightingShader.setFloat("pointLights[9].linear", 0.7);
        lightingShader.setFloat("pointLights[9].quadratic", 0.5);

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
        model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));	// it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        sceneModel.Draw(lightingShader);

        model = glm::translate(model, glm::vec3(-20.0f, 10.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
        //model = glm::translate(model, glm::vec3(currentFrame*10, 10.0f, currentFrame*10)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
        lightingShader.setMat4("model", model);
        ufoModel.Draw(lightingShader);

        //glActiveTexture(GL_TEXTURE2);
        //glBindTexture(GL_TEXTURE_2D, emissionMapStone);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(5.0f, 8.0f + sin(currentFrame) * 0.5, 15.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));	// it's a bit too big for our scene, so scale it down
        model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
        lightingShader.setMat4("model", model);
        stoneModel.Draw(lightingShader);
        //glBindTexture(GL_TEXTURE_2D, 0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, 1.0f, 2.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.003f, 0.003f, 0.003f));	// it's a bit too big for our scene, so scale it down
        //model = glm::translate(model, glm::vec3(currentFrame*10, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        lightingShader.setMat4("model", model);
        alienModel.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));	// it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        zombieModel.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 10.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        monsterModel.Draw(lightingShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(5.0f, 2.0f + sin(currentFrame) * 0.2, 15.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.003f, 0.003f, 0.003f));	// it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        portalModel.Draw(lightingShader);
        /*
        //glDisable(GL_TEXTURE0);
        //glDisable(GL_TEXTURE2);
        */

        /*
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 0.0f,1.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        landModel.Draw(ourShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        medievalHouseModel.Draw(ourShader);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseTerem);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, emissionTerem);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        teremModel.Draw(ourShader);
        //glDisable(GL_TEXTURE0);
        //glDisable(GL_TEXTURE1);
        //glDisable(GL_TEXTURE2);
        */

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
        for (unsigned int i = 0; i < 10; i++)
        {
            lightPredominat.setVec3("lightColorCube", pointLightColors[i]);
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.3f)); // Make it a smaller cube
            lightPredominat.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

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


// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format{};
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}