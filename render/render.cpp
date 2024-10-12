#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <thread>
#include <random>  // 新增

#include "stb_image.h"
#include "shader.h"
//#include "data.h"
#include "ortho_camera.h"
//#include "data_producer.h"
#include "data_producer_multi.h"
#include "render.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
Vertex calVertex(Vertex v1, Vertex v2, float p);


//使用新数据
bool useNewDataFetch = true;


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
OrthoCamera camera(glm::vec3(0.0f, 0.0f, 2.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// viewing frustum
float lef = -1.0f, rig = 1.0f;
float bottom = -0.75f, top = 0.75f;
float near = 0.1f, far = 100.0f;

// map border
//float leftBorder = -6.4f, rightBorder = 6.4f;
//float bottomBorder = -3.2f, topBorder = 3.2f;

//float leftBorder = -12.8f, rightBorder = 12.8f;
float leftBorder = -10.0f, rightBorder = 10.0f;
float bottomBorder = -10.0f, topBorder = 10.0f;

float maxHeight = topBorder / 4.0f;
float minHeight = topBorder / 32.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int seconds = 0;

float renderSpeed = 1.0f;

int previousKeyStateE = GLFW_RELEASE;
int previousKeyStateQ = GLFW_RELEASE;

Render::Render()
{
}

void Render::freeDataChunk(DataChunk dataChunk)
{
    delete[] dataChunk.indices;
    delete[] dataChunk.vertices;
}

void Render::render(std::queue<DataChunk>& dataChunkBuffer, std::mutex& dataChunkMutex)
{
    //InitialRenderStatus();
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "THE", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    string RenderPath = std::string(PROJECT_SOURCE_DIR) + "/render";
    //cout << RenderPath << endl;
    // build and compile our shader zprogram
    // ------------------------------------


    //Shader ourShader((RenderPath + "/shader/shader.vs").c_str(), (RenderPath + "/shader/shader.fs").c_str(), (RenderPath + "/shader/shader.gs").c_str());

    Shader ourShader((RenderPath + "/shader/shader.vs").c_str(), (RenderPath + "/shader/shader.fs").c_str(), (RenderPath + "/shader/shaderpoint.gs").c_str());
    Shader mapShader((RenderPath + "/shader/map.vs").c_str(), (RenderPath + "/shader/map.fs").c_str());

    glEnable(GL_PROGRAM_POINT_SIZE);
    stbi_set_flip_vertically_on_load(true);
    //float mapVertices[] = {
    //    // positions          // colors           
    //     6.4f,  3.2f, -1.0f,   1.0f, 1.0f, // top right
    //     6.4f, -3.2f, -1.0f,   1.0f, 0.0f, // bottom right
    //    -6.4f, -3.2f, -1.0f,   0.0f, 0.0f, // bottom left
    //    -6.4f,  3.2f, -1.0f,   0.0f, 1.0f  // top left 
    //};

    float mapVertices[] = {
        // positions          // colors           
         rightBorder,  topBorder, -1.0f,   1.0f, 1.0f, // top right
         rightBorder, bottomBorder, -1.0f,   1.0f, 0.0f, // bottom right
         leftBorder, bottomBorder, -1.0f,   0.0f, 0.0f, // bottom left
         leftBorder, topBorder, -1.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    unsigned int VBO[2], VAO[2], EBO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    glGenBuffers(2, EBO);


    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mapVertices), mapVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color  attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);



    // load and create a texture 
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    //unsigned char* mapData = stbi_load("../../../resources/map.jpg", &width, &height, &nrChannels, 0);

    //unsigned char* mapData = stbi_load((RenderPath + "/resources/worldmap.png").c_str(), &width, &height, &nrChannels, 0);
    unsigned char* mapData = stbi_load((RenderPath + "../../../worldmap.png").c_str(), &width, &height, &nrChannels, 0);
    if (mapData)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        else
            format = GL_RGB; // 默认格式

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, mapData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture from path: " << RenderPath + "../../../worldmap.png" << std::endl;
        glfwTerminate();
        return;
    }
    stbi_image_free(mapData);

    //gui initial
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    // Open multiple threads to parse data
    // -------------------------------------------------------------------------------
    Data data;
    /* for (int i = 0; i < 5; i++) {
         data.getDataAsyc();
     }*/


    float oneSec = 0.0f;
    float lastSec = 0.0f;
    float currentSec = 0.0f;
    int fps = 0;


    // 计算基础面积
    float base_area = (rig - lef) * (top - bottom) * 0.5;

    // 初始化随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // Get the point positions of the first and second seconds
        // -------------------------------------------------------------------------------
    DataChunk currentChunk;
    DataChunk nextChunk;

    vector<float> probabilities;

    if (useNewDataFetch) {
        currentChunk = data.getDataChunk(dataChunkBuffer, dataChunkMutex);

        for (int i = 0; i < currentChunk.unit_count; i++) {
            probabilities.push_back(dis(gen));
            
        }
        nextChunk = data.getDataChunk(dataChunkBuffer, dataChunkMutex);
    }
    else {
        currentChunk = data.getDataChunk();
        nextChunk = data.getDataChunk();
    }

    //cout << currentChunk.indices.size() << "cur " << currentChunk.vertices.size() << endl;
    //cout << nextChunk.indices.size() << "nex " << nextChunk.vertices.size() << endl;

    float currentDelta = 0.0f;
    while (!glfwWindowShouldClose(window))
    {

        /* GLenum err;
         while ((err = glGetError()) != GL_NO_ERROR) {
             std::cout << "OpenGL error: " << err << std::endl;
         }*/
         //gui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create UI for selecting render speed
        ImGui::Begin("Render Speed Options");
        ImGui::SetWindowSize(ImVec2(200, 150));
        if (ImGui::RadioButton("0.2x", renderSpeed == 0.2f)) {
            renderSpeed = 0.2f;
        }
        if (ImGui::RadioButton("0.5x", renderSpeed == 0.5f)) {
            renderSpeed = 0.5f;
        }
        if (ImGui::RadioButton("1x", renderSpeed == 1.0f)) {
            renderSpeed = 1.0f;
        }
        if (ImGui::RadioButton("2x", renderSpeed == 2.0f)) {
            renderSpeed = 2.0f;
        }
        if (ImGui::RadioButton("3x", renderSpeed == 3.0f)) {
            renderSpeed = 3.0f;
        }
        ImGui::End();



        fps++;
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        //cout << deltaTime << endl;
        lastFrame = currentFrame;


        currentDelta += deltaTime;

        // Get the point position of the next second
        // -------------------------------------------------------------------------------

        float renderTime = 1.0 / renderSpeed;
        if (currentDelta > renderTime) {
            freeDataChunk(currentChunk);
            currentChunk = nextChunk;
            if (useNewDataFetch) {
                nextChunk = data.getDataChunk(dataChunkBuffer, dataChunkMutex);
            }
            else {
                nextChunk = data.getDataChunk();
            }
            currentDelta = 0.0f;
            std::cout << "frame per step: " << fps << ", speed: " << renderSpeed << "x" << std::endl;
            fps = 0;
        }

        //cout << nextChunk.indices.size() << " " << nextChunk.vertices.size() << endl;

        // Interpolate according to time based on the point position of the previous second and the next second
        // -------------------------------------------------------------------------------
        std::vector<Vertex> vertices;
        /* for (int i = 0; i < currentChunk.vertices.size(); i++) {
            vertices.push_back(calVertex(currentChunk.vertices[i], nextChunk.vertices[i], currentDelta));
        }
        std::vector<int> indexLines = currentDelta < 0.5f ? currentChunk.indices : nextChunk.indices;*/

        // 计算当前可视面积
        float current_area = (rig - lef) * (top - bottom);
        float scale_factor = current_area / base_area;
        float skip_probability = (scale_factor - 1.0f) * 0.5f;  // 可调整的系数
        if (skip_probability < 0.0f) skip_probability = 0.0f;
        if (skip_probability > 0.6f) skip_probability = 0.7f;  // 最大跳过概率为70%

        // 输出跳过概率（可选）
        //std::cout << "Skip Probability: " << skip_probability << std::endl;

        for (int i = 0; i < currentChunk.unit_count; i++) {
            if (probabilities[i] > skip_probability) {  // 随机决定是否渲染该点
                vertices.push_back(calVertex(currentChunk.vertices[i], nextChunk.vertices[i], currentDelta / renderTime));
            }
        }

        std::vector<int> indexLines = currentDelta < 0.5f
            ? std::vector<int>(currentChunk.indices, currentChunk.indices + currentChunk.unit_count)
            : std::vector<int>(nextChunk.indices, nextChunk.indices + nextChunk.unit_count);

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units
        glBindTexture(GL_TEXTURE_2D, texture);

        // draw map
        // -------------------------------------------------------------------------------
        mapShader.use();

        //lef = -1.0f, rig = 1.0f;
        //bottom = -float(SCR_HEIGHT) / float(SCR_WIDTH), top = float(SCR_HEIGHT) / float(SCR_WIDTH);

        /*lef = -1.0f, rig = 1.0f;
        bottom = -float(SCR_HEIGHT) / float(SCR_WIDTH), top = float(SCR_HEIGHT) / float(SCR_WIDTH);*/
        glm::mat4 projection = glm::ortho(lef, rig, bottom, top, near, far);
        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        mapShader.setMat4("projection", projection);
        mapShader.setMat4("view", view);
        mapShader.setMat4("model", model);


        glBindVertexArray(VAO[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // activate shader
        ourShader.use();

        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        ourShader.setMat4("model", model);



        // draw points
        // -------------------------------------------------------------------------------
        float time1 = static_cast<float>(glfwGetTime());
        glBindVertexArray(VAO[1]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indexLines.size(), &indexLines[0], GL_DYNAMIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        // color  attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, status));
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 1, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, camps));
        glEnableVertexAttribArray(3);

        glDrawArrays(GL_POINTS, 0, vertices.size());

        float time2 = static_cast<float>(glfwGetTime());
        //cout << "draw points time : " << time2 - time1 << endl;

        // draw lines
        // -------------------------------------------------------------------------------
       /* glDrawElements(GL_LINES, indexLines.size(), GL_UNSIGNED_INT, 0);


        time2 = static_cast<float>(glfwGetTime());*/
        //cout << "draw lines time : " << time2 - time1 << endl;

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteBuffers(2, EBO);  // 确保EBO也被删除

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    int currentKeyStateE = glfwGetKey(window, GLFW_KEY_E);
    if (currentKeyStateE == GLFW_PRESS && previousKeyStateE == GLFW_RELEASE) {
        // 按键 E 刚被按下
        if (renderSpeed < 3.0f) {
            renderSpeed += 0.5f;
            std::cout << "Render Speed: " << renderSpeed << "x" << std::endl;
        }
    }
    // 更新按键状态
    previousKeyStateE = currentKeyStateE;

    // 检测 Q 键
    int currentKeyStateQ = glfwGetKey(window, GLFW_KEY_Q);
    if (currentKeyStateQ == GLFW_PRESS && previousKeyStateQ == GLFW_RELEASE) {
        // 按键 Q 刚被按下
        if (renderSpeed > 0.5f) {
            renderSpeed -= 0.5f;
            std::cout << "Render Speed: " << renderSpeed << "x" << std::endl;
        }
    }
    // 更新按键状态
    previousKeyStateQ = currentKeyStateQ;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime, topBorder, top);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime, bottomBorder, bottom);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime, leftBorder, lef);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime, rightBorder, rig);


}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.

    glViewport(0, 0, width, height);
    //cout << "width: " << width << "height: " << height << endl;
    bottom = float(height) / float(width) * lef;
    top = float(height) / float(width) * rig;
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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
    //glm::vec3 Position = camera.getPosition();
    //float posX = camera.getAbs(Position.x);
    //float posY = camera.getAbs(Position.y);
    ////cout << "x : " << xoffset << "y : " << yoffset << endl;
    //if (yoffset < 0.f && (posX - yoffset * 0.1f + rig > rightBorder || posY - yoffset * 0.1f + top > topBorder)) {
    //    return;
    //}
    //if (top - yoffset * topBorder * 0.01 >= maxHeight || top - yoffset * topBorder * 0.01 <= minHeight) return;
    //lef += yoffset * rightBorder * 0.01;
    //rig -= yoffset * rightBorder * 0.01;
    //top -= yoffset * topBorder * 0.01;
    //bottom += yoffset * topBorder * 0.01;

    float zoomFactor = 0.1f;
    float zoomAmount = yoffset * zoomFactor;

    // Calculate the aspect ratio of the viewport
    float aspectRatio = static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT);

    // Adjust the orthographic projection bounds
    float width = rig - lef;
    float height = top - bottom;

    // Update the bounds based on the zoom amount
    width -= zoomAmount;
    height -= zoomAmount / aspectRatio;


    // Ensure width and height remain positive
    if (width <= 0.0f) width = 0.1f;
    if (height <= 0.0f) height = 0.1f;

    // Ensure height is within minHeight and maxHeight
    if (height > maxHeight)
    {
        height = maxHeight;
        return;
    }
    if (height < minHeight)
    {
        height = minHeight;
        return;
    }

    // Maintain aspect ratio
    float centerX = (lef + rig) / 2.0f;
    float centerY = (bottom + top) / 2.0f;

    lef = centerX - width / 2.0f;
    rig = centerX + width / 2.0f;
    bottom = centerY - height / 2.0f;
    top = centerY + height / 2.0f;

    // Ensure the view frustum does not exceed the map borders
    if (lef < leftBorder) lef = leftBorder;
    if (rig > rightBorder) rig = rightBorder;
    if (bottom < bottomBorder) bottom = bottomBorder;
    if (top > topBorder) top = topBorder;

    // cout << "LEF : " << lef << "RIG : " << rig << endl;
     //camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

Vertex calVertex(Vertex v1, Vertex v2, float p) {
    glm::vec3 v = (v1.position + (v2.position - v1.position) * p) * 4.0f;
    Vertex vertex(v, v1.color, v1.camps, v1.status);
    return vertex;
}
