//// Render.h
//#ifndef RENDER_H
//#define RENDER_H
//
//#include <queue>
//#include <mutex>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <iostream>
//#include <thread>
//#include <random>  
//
//#include "stb_image.h"
//#include "shader.h"
////#include "data.h"
//#include "ortho_camera.h"
////#include "data_producer.h"
//#include "data_producer_multi.h"
//#include "render.h"
//
//
//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"
//#include "model.h"
//
//
//
//class Render {
//public:
//    Render();
//    void render(std::queue<int>& queryIdBuffer, std::mutex& queryIdMutex,
//        std::queue<DataChunk1>& dataChunkBuffer, std::mutex& dataChunkMutex);
//    void freeDataChunk(DataChunk1 dataChunk);
//
//private:
//    // 初始化相关
//    bool initWindow();
//    bool initGLAD();
//    void initImGui();
//    void loadShaders(const std::string& renderPath);
//    void InitialData(std::queue<DataChunk>& dataChunkBuffer, std::mutex& dataChunkMutex);
//    void InitialBuffer();
//    void setupBuffers();
//    void loadTextures(const std::string& renderPath);
//
//    // 渲染相关
//    void processImgui();
//    void renderMap(const Shader& shader, unsigned int VAO);
//    void renderEntities(std::vector<VertexPoint>& instances);
//    void renderGrid(const Shader& shader, unsigned int VAO);
//    void cleanup();
//
//    // 数据处理
//    DataChunk1 getNextChunk(Data& data, bool useNewDataFetch,
//        std::queue<DataChunk1>& dataChunkBuffer, std::mutex& dataChunkMutex);
//    void updateInstances(std::vector<VertexPoint>& vertices,
//        float base_area);
//
//    // 工具函数
//    void processInput(GLFWwindow* window);
//    void handleQuery(int queryID, const DataChunk1& currentChunk, const DataChunk1& nextChunk,
//        float currentDelta, float renderTime, std::queue<int>& queryIdBuffer,
//        std::mutex& queryIdMutex, std::string& positionInfo, std::string& gridInfo);
//
//    // 成员变量
//    // 
//    GLFWwindow* window;
//
//    bool useNewDataFetch = true;
//
//
//    // 窗口设置
//    const unsigned int SCR_WIDTH = 800;
//    const unsigned int SCR_HEIGHT = 600;
//
//    //shader
//    Shader ourShader;
//    Shader gridShader;
//    Shader mapShader;
//    Shader entityShader;
//
//	// 实体
//    Model rock;
//
//
//	// 数据
//    float mapVertices[20];
//    unsigned int indices[20];
//    std::vector<float> gridVertices;
//
//	DataChunk1 currentChunk;
//	DataChunk1 nextChunk;
//
//    vector<float> probabilities;
//
//    // 相机控制
//    OrthoCamera camera;
//    float lastX = SCR_WIDTH / 2.0f;
//    float lastY = SCR_HEIGHT / 2.0f;
//    bool firstMouse = true;
//
//    // 视图参数
//    float lef = -1.0f, rig = 1.0f;
//    float bottom = -0.75f, top = 0.75f;
//    float near1 = 0.1f, far1 = 100.0f;
//
//    // 地图边界
//    float leftBorder = -10.0f, rightBorder = 10.0f;
//    float bottomBorder = -10.0f, topBorder = 10.0f;
//
//    // 其他参数
//    float earthRadius = 8.5f;
//    float gridLineWidth = 0.4f;
//    float transparency = 0.01f;
//    float renderSpeed = 1.0f;
//
//    //gui
//    int queryID = 0;
//    char inputBuffer[10] = "";
//    bool validID = false;
//    std::string positionInfo;
//    std::string gridInfo;
//
//    Data data;
//
//    // OpenGL资源
//    unsigned int instanceVBO;
//    unsigned int gridVAO, gridVBO;
//    unsigned int mapVAO, mapVBO, mapEBO;
//    unsigned int textureID;
//
//
//};
//
//// 回调函数声明
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//VertexPoint calVertexPoint(VertexPoint v1, VertexPoint v2, float p, bool highLight);
//
//#endif // RENDER_H