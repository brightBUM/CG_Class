#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include "Shader.h"
#include <iostream>
#include"Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void MousePosCallback(GLFWwindow* window, double xPos, double yPos);
void MouseScrollCallback(GLFWwindow* window, double xPos, double yPos);

void processInput(GLFWwindow* window);

void FPSCounter(GLFWwindow* window);

void LoadTexture(unsigned int& texture, const char* path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//global variables
double deltaTime = 0.0;
double lastFrame = 0.0;

double fps = 0.0;
double fpsTimer = 0.0;
double fpsUpdateInterval = 0.5; // half a second
int frames = 0;

glm::vec3 camPos = glm::vec3(0.0f,0.0f,-1.5f);
float cameraSpeed = 0.5f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


float mixValue = 0.0f;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 2.0f);
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, MousePosCallback);
    glfwSetScrollCallback(window, MouseScrollCallback);
    //glfwSwapInterval(0); // disable vsync

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("Shaders/default.vert", "Shaders/default.frag"); // you can name your shader files however you like
    Shader lightShader("Shaders/light.vert", "Shaders/light.frag");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    
    //triangle - pos,col,uv
    float vertices[] = {
        // positions         // colors         //Tex Coords            
        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f,1.0f, // top left - 0
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f,1.0f,  // top right - 1
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f,0.0f,  // bottom right - 2
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f,0.0f  // bottom left -3
    };
    unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
    };
    //cube - pos , col
    float cubeVertices[] = {
        // position           // color
        -0.5f,-0.5f,-0.5f,    1.0f,0.0f,0.0f,   // 0
         0.5f,-0.5f,-0.5f,    0.0f,1.0f,0.0f,   // 1
         0.5f, 0.5f,-0.5f,    0.0f,0.0f,1.0f,   // 2
        -0.5f, 0.5f,-0.5f,    1.0f,1.0f,0.0f,   // 3

        -0.5f,-0.5f, 0.5f,    1.0f,0.0f,1.0f,   // 4
         0.5f,-0.5f, 0.5f,    0.0f,1.0f,1.0f,   // 5
         0.5f, 0.5f, 0.5f,    1.0f,1.0f,1.0f,   // 6
        -0.5f, 0.5f, 0.5f,    0.3f,0.3f,0.3f    // 7
    };
    unsigned int cubeIndices[] = {
        // Front face
        0, 1, 2,
        2, 3, 0,

        // Right face
        1, 5, 6,
        6, 2, 1,

        // Back face
        5, 4, 7,
        7, 6, 5,

        // Left face
        4, 0, 3,
        3, 7, 4,

        // Top face
        3, 2, 6,
        6, 7, 3,

        // Bottom face
        4, 5, 1,
        1, 0, 4
    };

    //cube - pos,uv
    float cubeVertices2[] = {
        // ===== FRONT FACE (normal: 0, 0, 1) =====
    //    position              normal           uv
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,    0.0f, 1.0f,

    // ===== BACK FACE (normal: 0, 0, -1) =====
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,    0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,   0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,    0.0f, 0.0f, -1.0f,   1.0f, 1.0f,

    // ===== LEFT FACE (normal: -1, 0, 0) =====
    -0.5f, -0.5f, -0.5f,    -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,    -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,    -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,    -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,

    // ===== RIGHT FACE (normal: 1, 0, 0) =====
     0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,    1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f,    1.0f, 1.0f,

     // ===== TOP FACE (normal: 0, 1, 0) =====
     -0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f,    0.0f, 1.0f,
      0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f,
      0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
     -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f,

     // ===== BOTTOM FACE (normal: 0, -1, 0) =====
     -0.5f, -0.5f,  0.5f,    0.0f, -1.0f, 0.0f,   0.0f, 0.0f,
      0.5f, -0.5f,  0.5f,    0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
      0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
     -0.5f, -0.5f, -0.5f,    0.0f, -1.0f, 0.0f,   0.0f, 1.0f
    };
    unsigned int cubeIndices2[] = {
        // FRONT
        0, 1, 2,  2, 3, 0,

        // BACK
        4, 5, 6,  6, 7, 4,

        // LEFT
        8, 9, 10, 10, 11, 8,

        // RIGHT
        12, 13, 14, 14, 15, 12,

        // TOP
        16, 17, 18, 18, 19, 16,

        // BOTTOM
        20, 21, 22, 22, 23, 20
    };


    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices2), cubeVertices2, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices2), cubeIndices2, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    //create a light source //another cube
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    unsigned int texture1;
    unsigned int texture2;
    LoadTexture(texture1, "Textures/cat_open.png");
    LoadTexture(texture2, "Textures/cat_close.png");

    ourShader.use();
    ourShader.setInt("texSampler1", 0);
    ourShader.use();
    ourShader.setInt("texSampler2", 1);


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST); // ENABLE DEPTH BUFFER

    glEnable(GL_BLEND); //enable Blend
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // render loop 
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        FPSCounter(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color and depth buffer
        
        glm::vec3 lightColor = glm::vec3(1.0f);
        //transformation
        
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

        view = glm::translate(view, camPos);

        //camera rotation
        glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 direction = glm::vec3(5.0f, 0.0f, 0.0f) - camPos;
        glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);

        //look at - pos,target direction and up vector
        view = camera.GetViewMatrix();
        

        //proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
        proj = glm::perspective(glm::radians(camera.Zoom), 
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        ourShader.use();
        ourShader.SetMat4("view", view);
        ourShader.SetMat4("proj", proj);
        ourShader.setFloat("mixValue", mixValue);
        ourShader.SetVec3("lightColor", lightColor);
        ourShader.SetVec3("lightPos", lightPos);
        ourShader.SetVec3("camPos", camera.Position);
        


        // render the triangle
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);
        // 6 faces * 2 triangles * 3 vertices = 36

        for (float i = 0.0f; i <= 3.0f; i+=1.0f)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(i,0.0f,0.0f));
            model = glm::scale(model, glm::vec3(0.5, 0.5f, 0.5f));
            ourShader.use();
            ourShader.SetMat4("model", model);
            ourShader.setFloat("material.specular", 0.2f*(i+1));
            ourShader.setFloat("material.shininess", 8.0f*(i+1));
            ourShader.setFloat("material.ambient", 0.2f);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        //drawing light source
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.1f));


        lightShader.use();
        lightShader.SetMat4("model", lightModel);
        lightShader.SetMat4("view", view);
        lightShader.SetMat4("proj", proj);
        lightShader.SetVec3("lightColor", lightColor);

        glBindVertexArray(lightVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void LoadTexture(unsigned int& texture,const char* path)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    stbi_set_flip_vertically_on_load(true);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, STBI_rgb_alpha);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture : "<<path << std::endl;
    }

    stbi_image_free(data);
}

void FPSCounter(GLFWwindow* window)
{
    // --- DELTA TIME ---
    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // --- FPS COUNTER ---
    frames++;
    fpsTimer += deltaTime;

    if (fpsTimer >= fpsUpdateInterval)   // update every 0.5s
    {
        fps = frames / fpsTimer;         // true FPS
        frames = 0;
        fpsTimer = 0.0;

        std::string title =
            "CG_Class | FPS: " + std::to_string((int)fps) +
            " | delta: " + std::to_string(deltaTime);

        glfwSetWindowTitle(window, title.c_str());
    }
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS )
        camera.ProcessKeyboard(FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        lightPos += glm::vec3(-1.0f, 0.0f, 0.0f) * camera.MovementSpeed * static_cast<float>(deltaTime);

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        lightPos += glm::vec3(1.0f, 0.0f, 0.0f) * camera.MovementSpeed * static_cast<float>(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        lightPos += glm::vec3(0.0f, 0.0f, -1.0f) * camera.MovementSpeed * static_cast<float>(deltaTime);

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        lightPos += glm::vec3(0.0f, 0.0f, 1.0f) * camera.MovementSpeed * static_cast<float>(deltaTime);
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    std::cout << "size : " << width << " , " << height << std::endl;
}
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        mixValue = 1.0f;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        mixValue = 0.0f;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        std::cout << "Right mouse pressed\n";
}
void MousePosCallback(GLFWwindow* window, double xPos, double yPos)
{
    float xpos = static_cast<float>(xPos);
    float ypos = static_cast<float>(yPos);

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
void MouseScrollCallback(GLFWwindow* window, double xPos, double yPos)
{
    std::cout << "xScroll : " << xPos << " ,yPos : " << yPos << std::endl;
    camera.ProcessMouseScroll(static_cast<float>(yPos));
    std::cout << "zoom : " << camera.Zoom << std::endl;
}