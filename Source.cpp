#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include "Shader.h"
#include <iostream>
#include"Camera.h"
#include<vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;
using namespace glm;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void MousePosCallback(GLFWwindow* window, double xPos, double yPos);
void MouseScrollCallback(GLFWwindow* window, double xPos, double yPos);
static glm::vec2 CursorToNDC(GLFWwindow* window, double xPos, double yPos);

void processInput(GLFWwindow* window);

void FPSCounter(GLFWwindow* window);
glm::vec2 Lerp(glm::vec2 ptA, glm::vec2 ptB, float t);
vec2 RecursiveLerp(const vector<vec2>& points, float t);

std::vector<vec2> points = {
    glm::vec2(0.7f, -0.5f),
    glm::vec2(0.2f, 0.7f),
    glm::vec2(-0.2f, 0.7f),
    glm::vec2(-0.7f, -0.5f)
    //glm::vec2(0.5f,0.8f)
};

// settings
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 800;

//global variables
double deltaTime = 0.0;
double lastFrame = 0.0;

double fps = 0.0;
double fpsTimer = 0.0;
double fpsUpdateInterval = 0.5; // half a second
int frames = 0;

glm::vec3 camPos = glm::vec3(0.0f, 0.0f, -1.5f);
float cameraSpeed = 0.5f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


float mixValue = 0.0f;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 2.0f);

bool mouseClick = false;
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

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
    //glfwSetScrollCallback(window, MouseScrollCallback);
    //glfwSwapInterval(0); // disable vsync

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glEnable(GL_DEPTH_TEST); // ENABLE DEPTH BUFFER

    //glEnable(GL_BLEND); //enable Blend
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::vec3 colors[] = { glm::vec3(1.0f,0.0f,0.0f),
                            glm::vec3(1.0f,1.0f,0.0f),
                            glm::vec3(0.0f,1.0f,0.0f) ,
                            glm::vec3(0.5f,0.8f,0.5f)
    };
    

    std::vector<vec2> curvePoints;
    int samplePoints = 12;
    float offset = 0.1f;
    int lineSize = 50;
    float moveSpeed = 8.0f;
    for (int i = 0; i < samplePoints; i++)
    {
        curvePoints.push_back(glm::vec2(0.0f, 0.0f));
    }
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
        glClear(GL_COLOR_BUFFER_BIT); // clear color and depth buffer

        /*glPointSize(10.0f);

        glBegin(GL_POINTS);
        glColor3f(1.0f, 1.0f, 1.0f);
        for (int i = 0; i < points.size(); i++)
        {
            glVertex2f(points[i].x, points[i].y);
        }
        glEnd();*/

        

        glPointSize(10.0f);

        glBegin(GL_POINTS);
        glColor3f(0.0f, 1.0f, 0.5f);

        double mx, my;
        glfwGetCursorPos(window, &mx, &my);
        //curvePoints[0] = Lerp(curvePoints[0], CursorToNDC(window, mx, my),deltaTime*moveSpeed);
        curvePoints[0] = CursorToNDC(window, mx, my);

        for (int i = 1; i < samplePoints; i++)
        {
            vec2 dir = curvePoints[i - 1] - curvePoints[i];
            vec2 target = curvePoints[i - 1] + (-glm::normalize(dir) * offset);
            //curvePoints[i] = Lerp(curvePoints[i],target, deltaTime * moveSpeed);
            curvePoints[i] = target;

            glVertex2f(curvePoints[i].x, curvePoints[i].y);
        }
        glEnd();

        glLineWidth(5.0f);
        glBegin(GL_LINE_STRIP);
        glColor3f(1.0f, 0.5f, 0.0f);
        for (int i = 0; i < curvePoints.size(); i++)
        {
            glVertex2f(curvePoints[i].x, curvePoints[i].y);
        }


        // Stop defining the triangle
        glEnd();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}

glm::vec2 Lerp(glm::vec2 ptA, glm::vec2 ptB, float t)
{
    return (1 - t) * ptA + t * ptB;
}

vec2 RecursiveLerp(const vector<vec2>& points, float t)
{
    if (points.size() == 1)
        return points[0];

    vector<vec2> lerpPoints;

    for (int i = 0; i < points.size() - 1; i++)
    {
        auto point = Lerp(points[i], points[i + 1], t);
        lerpPoints.push_back(point);
    }
    
    return RecursiveLerp(lerpPoints, t);

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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
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
static int draggedIndex = -1;
static float pickRadius = 0.08f; // tune

static glm::vec2 CursorToNDC(GLFWwindow* window, double xPos, double yPos)
{
    int fbW, fbH;
    glfwGetFramebufferSize(window, &fbW, &fbH);

    float x = (float)((xPos / (double)fbW) * 2.0 - 1.0);
    float y = (float)(1.0 - (yPos / (double)fbH) * 2.0); // flip Y
    return { x, y };
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT) return;

    if (action == GLFW_PRESS)
    {
        mouseClick = true;

        // pick once
        double mx, my;
        glfwGetCursorPos(window, &mx, &my);
        glm::vec2 m = CursorToNDC(window, mx, my);

        draggedIndex = -1;
        float best = pickRadius; // only accept within radius
        for (int i = 0; i < (int)points.size(); i++)
        {
            float d = glm::distance(points[i], m);
            if (d < best)
            {
                best = d;
                draggedIndex = i;
            }
        }
    }
    else if (action == GLFW_RELEASE)
    {
        mouseClick = false;
        draggedIndex = -1;
    }
}

void MousePosCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (!mouseClick || draggedIndex < 0) return;

    glm::vec2 m = CursorToNDC(window, xPos, yPos);
    points[draggedIndex] = m;
}
void MouseScrollCallback(GLFWwindow* window, double xPos, double yPos)
{
    std::cout << "xScroll : " << xPos << " ,yPos : " << yPos << std::endl;
    camera.ProcessMouseScroll(static_cast<float>(yPos));
    std::cout << "zoom : " << camera.Zoom << std::endl;
}