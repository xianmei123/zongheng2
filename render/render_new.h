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
//    // ��ʼ�����
//    bool initWindow();
//    bool initGLAD();
//    void initImGui();
//    void loadShaders(const std::string& renderPath);
//    void InitialData(std::queue<DataChunk>& dataChunkBuffer, std::mutex& dataChunkMutex);
//    void InitialBuffer();
//    void setupBuffers();
//    void loadTextures(const std::string& renderPath);
//
//    // ��Ⱦ���
//    void processImgui();
//    void renderMap(const Shader& shader, unsigned int VAO);
//    void renderEntities(std::vector<VertexPoint>& instances);
//    void renderGrid(const Shader& shader, unsigned int VAO);
//    void cleanup();
//
//    // ���ݴ���
//    DataChunk1 getNextChunk(Data& data, bool useNewDataFetch,
//        std::queue<DataChunk1>& dataChunkBuffer, std::mutex& dataChunkMutex);
//    void updateInstances(std::vector<VertexPoint>& vertices,
//        float base_area);
//
//    // ���ߺ���
//    void processInput(GLFWwindow* window);
//    void handleQuery(int queryID, const DataChunk1& currentChunk, const DataChunk1& nextChunk,
//        float currentDelta, float renderTime, std::queue<int>& queryIdBuffer,
//        std::mutex& queryIdMutex, std::string& positionInfo, std::string& gridInfo);
//
//    // ��Ա����
//    // 
//    GLFWwindow* window;
//
//    bool useNewDataFetch = true;
//
//
//    // ��������
//    const unsigned int SCR_WIDTH = 800;
//    const unsigned int SCR_HEIGHT = 600;
//
//    //shader
//    Shader ourShader;
//    Shader gridShader;
//    Shader mapShader;
//    Shader entityShader;
//
//	// ʵ��
//    Model rock;
//
//
//	// ����
//    float mapVertices[20];
//    unsigned int indices[20];
//    std::vector<float> gridVertices;
//
//	DataChunk1 currentChunk;
//	DataChunk1 nextChunk;
//
//    vector<float> probabilities;
//
//    // �������
//    OrthoCamera camera;
//    float lastX = SCR_WIDTH / 2.0f;
//    float lastY = SCR_HEIGHT / 2.0f;
//    bool firstMouse = true;
//
//    // ��ͼ����
//    float lef = -1.0f, rig = 1.0f;
//    float bottom = -0.75f, top = 0.75f;
//    float near1 = 0.1f, far1 = 100.0f;
//
//    // ��ͼ�߽�
//    float leftBorder = -10.0f, rightBorder = 10.0f;
//    float bottomBorder = -10.0f, topBorder = 10.0f;
//
//    // ��������
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
//    // OpenGL��Դ
//    unsigned int instanceVBO;
//    unsigned int gridVAO, gridVBO;
//    unsigned int mapVAO, mapVBO, mapEBO;
//    unsigned int textureID;
//
//
//};
//
//// �ص���������
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//VertexPoint calVertexPoint(VertexPoint v1, VertexPoint v2, float p, bool highLight);
//
//#endif // RENDER_H