//// Render.cpp
//#include "render_new.h"
//
//
//// 实现所有成员函数...
//
//Render::Render()
//{
//}
//
//void Render::render(std::queue<int>& queryIdBuffer, std::mutex& queryIdMutex,
//    std::queue<DataChunk>& dataChunkBuffer, std::mutex& dataChunkMutex) {
//    // 初始化
//    if (!initWindow()) return;
//    if (!initGLAD()) return;
//
//    std::string renderPath = std::string(PROJECT_SOURCE_DIR) + "/render";
//    loadShaders(renderPath);
//    setupBuffers();
//	InitialData(dataChunkBuffer, dataChunkMutex);
//    InitialBuffer();
//
//    loadTextures(renderPath);
//    initImGui(window);
//
//    rock((renderPath + "/resources/rock/rock.obj").c_str());
//
//    float base_area = (rig - lef) * (top - bottom) * 0.5;
//
//    float oneSec = 0.0f;
//    float lastSec = 0.0f;
//    float currentSec = 0.0f;
//    int fps = 0;
//
//    
//    float currentDelta = 0.0f;
//
//    // 主循环
//    while (!glfwWindowShouldClose(window)) {
//        // 处理输入
//        processInput(window);
//
//        fps++;
//        float currentFrame = static_cast<float>(glfwGetTime());
//        deltaTime = currentFrame - lastFrame;
//        //cout << deltaTime << endl;
//        lastFrame = currentFrame;
//
//        float renderTime = 1.0 / renderSpeed;
//
//        processImgui();
//
//
//        if (currentDelta > renderTime) {
//            freeDataChunk(currentChunk);
//            currentChunk = nextChunk;
//            nextChunk = data.getDataChunk(dataChunkBuffer, dataChunkMutex);
//            currentDelta = 0.0f;
//            std::cout << "frame per step: " << fps << ", speed: " << renderSpeed << "x" << std::endl;
//            fps = 0;
//        }
//        std::vector<VertexPoint> vertices;
//
//        
//        updateInstances(vertices, base_area);
//
//        // 
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        renderMap(mapShader, mapVAO);
//        renderEntities(vertices);
//        renderGrid(gridShader, gridVAO);
//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//        // -------------------------------------------------------------------------------
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//
//       
//    }
//
//    // 清理
//    cleanup();
//}
//
//void Render::freeDataChunk(DataChunk1 dataChunk)
//{
//    delete[] dataChunk.indices;
//    delete[] dataChunk.vertices;
//}
//
//bool Render::initWindow()
//{
//    // glfw: initialize and configure
//    // ------------------------------
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//    // glfw window creation
//    // --------------------
//    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "THE", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return false;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//    glfwSetCursorPosCallback(window, mouse_callback);
//    glfwSetScrollCallback(window, scroll_callback);
//    return true;
//}
//
//bool Render::initGLAD()
//{
//    // ---------------------------------------
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return false;
//    }
//    return true;
//}
//
//void Render::initImGui()
//{
//    //gui initial
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
//    ImGui::StyleColorsDark();
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui_ImplOpenGL3_Init("#version 330");
//}
//
//void Render::loadShaders(const std::string& renderPath)
//{
//    ourShader((renderPath + "/shader/shader.vs").c_str(), (renderPath + "/shader/shader.fs").c_str(), (renderPath + "/shader/shaderpoint.gs").c_str());
//    mapShader((renderPath + "/shader/map.vs").c_str(), (renderPath + "/shader/map.fs").c_str());
//    // Shader for grid lines
//    gridShader((renderPath + "/shader/grid.vs").c_str(), (renderPath + "/shader/grid.fs").c_str());
//    entityShader((renderPath + "/shader/obj.vs").c_str(),
//        (renderPath + "/shader/obj.fs").c_str());
//}
//
//void Render::InitialData(std::queue<DataChunk>& dataChunkBuffer, std::mutex& dataChunkMutex)
//{
//    mapVertices = {
//        // positions          // colors           
//         rightBorder,  topBorder, -1.0f,   1.0f, 1.0f, // top right
//         rightBorder, bottomBorder, -1.0f,   1.0f, 0.0f, // bottom right
//         leftBorder, bottomBorder, -1.0f,   0.0f, 0.0f, // bottom left
//         leftBorder, topBorder, -1.0f,   0.0f, 1.0f  // top left 
//    };
//    indices = {
//        0, 1, 3, // first triangle
//        1, 2, 3  // second triangle
//    };
//
//    // Horizontal lines
//    for (float y = bottomBorder; y <= topBorder; y += gridLineWidth) {
//        gridVertices.push_back(leftBorder);
//        gridVertices.push_back(y);
//        gridVertices.push_back(-0.5f);
//
//        gridVertices.push_back(rightBorder);
//        gridVertices.push_back(y);
//        gridVertices.push_back(-0.5f);
//    }
//
//    // Vertical lines
//    for (float x = leftBorder; x <= rightBorder; x += gridLineWidth) {
//        gridVertices.push_back(x);
//        gridVertices.push_back(bottomBorder);
//        gridVertices.push_back(-0.5f);
//
//        gridVertices.push_back(x);
//        gridVertices.push_back(topBorder);
//        gridVertices.push_back(-0.5f);
//    }
//
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_real_distribution<> dis(0.0, 1.0);
//
//    // Get the point positions of the first and second seconds
//        // -------------------------------------------------------------------------------
//
//    
//    currentChunk = data.getDataChunk(dataChunkBuffer, dataChunkMutex);
//
//    for (int i = 0; i < currentChunk.unit_count; i++) {
//        probabilities.push_back(dis(gen));
//
//    }
//    nextChunk = data.getDataChunk(dataChunkBuffer, dataChunkMutex);
//  
//}
//
//void Render::InitialBuffer()
//{
//	// instance VBO
//    glGenBuffers(1, &instanceVBO);
//    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPoint) * amount, nullptr, GL_DYNAMIC_DRAW);
//
//    for (unsigned int i = 0; i < rock.meshes.size(); i++) {
//        unsigned int VAO = rock.meshes[i].VAO;
//        glBindVertexArray(VAO);
//
//        // 绑定实例化VBO
//        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
//
//        // 位置属性 (vec3)
//        glEnableVertexAttribArray(3);
//        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPoint), (void*)offsetof(VertexPoint, position));
//
//        // 颜色属性 (vec3)
//        glEnableVertexAttribArray(4);
//        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPoint), (void*)offsetof(VertexPoint, color));
//
//        // 状态属性 (int)
//        glEnableVertexAttribArray(5);
//        glVertexAttribPointer(5, 1, GL_INT, GL_FALSE, sizeof(VertexPoint), (void*)offsetof(VertexPoint, status));
//
//        // 阵营属性 (int)
//        glEnableVertexAttribArray(6);
//        glVertexAttribPointer(6, 1, GL_INT, GL_FALSE, sizeof(VertexPoint), (void*)offsetof(VertexPoint, camps));
//
//        // 设置属性除数
//        glVertexAttribDivisor(3, 1);
//        glVertexAttribDivisor(4, 1);
//        glVertexAttribDivisor(5, 1);
//        glVertexAttribDivisor(6, 1);
//
//        glBindVertexArray(0);
//    }
//
//	//grid VAO
//    glGenVertexArrays(1, &gridVAO);
//    glGenBuffers(1, &gridVBO);
//
//    glBindVertexArray(gridVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
//    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), &gridVertices[0], GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//	//map VAO
//    glGenVertexArrays(1, &mapVAO);
//    glGenBuffers(1, &mapVBO);
//    glBindVertexArray(mapVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, mapVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(mapVertices), mapVertices, GL_STATIC_DRAW);
//
//    // position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    // color  attribute
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//
//
//
//
//
//
//}
//
//void Render::setupBuffers()
//{
//    glEnable(GL_PROGRAM_POINT_SIZE);
//    stbi_set_flip_vertically_on_load(true);
//}
//
//void Render::loadTextures(const std::string& renderPath)
//{
//    // load and create a texture 
//    // -------------------------
//   
//    glGenTextures(1, &textureID);
//    glBindTexture(GL_TEXTURE_2D, textureID); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
//    // set the texture wrapping parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    // set texture filtering parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    // load image, create texture and generate mipmaps
//    int width, height, nrChannels;
//    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
//    //unsigned char* mapData = stbi_load("../../../resources/map.jpg", &width, &height, &nrChannels, 0);
//
//    unsigned char* mapData = stbi_load((RenderPath + "/resources/worldmap.jpg").c_str(), &width, &height, &nrChannels, 0);
//    //unsigned char* mapData = stbi_load((RenderPath + "/resources/newmap1.png").c_str(), &width, &height, &nrChannels, 0);
//    if (mapData)
//    {
//        GLenum format;
//        if (nrChannels == 1)
//            format = GL_RED;
//        else if (nrChannels == 3)
//            format = GL_RGB;
//        else if (nrChannels == 4)
//            format = GL_RGBA;
//        else
//            format = GL_RGB; // ????
//
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, mapData);
//        glGenerateMipmap(GL_TEXTURE_2D);
//    }
//    else
//    {
//        std::cerr << "Failed to load texture from path: " << RenderPath + "../../../worldmap.png" << std::endl;
//        glfwTerminate();
//        return;
//    }
//    stbi_image_free(mapData);
//}
//
//void Render::processImgui()
//{
//    ImGui_ImplOpenGL3_NewFrame();
//    ImGui_ImplGlfw_NewFrame();
//    ImGui::NewFrame();
//
//    // Create UI for selecting render speed
//
//    ImGui::Begin("Render Speed Options");
//    ImGui::SetWindowSize(ImVec2(200, 150));
//    if (ImGui::RadioButton("0.2x", renderSpeed == 0.2f)) {
//        renderSpeed = 0.2f;
//    }
//    if (ImGui::RadioButton("0.5x", renderSpeed == 0.5f)) {
//        renderSpeed = 0.5f;
//    }
//    if (ImGui::RadioButton("1x", renderSpeed == 1.0f)) {
//        renderSpeed = 1.0f;
//    }
//    if (ImGui::RadioButton("2x", renderSpeed == 2.0f)) {
//        renderSpeed = 2.0f;
//    }
//    if (ImGui::RadioButton("3x", renderSpeed == 3.0f)) {
//        renderSpeed = 3.0f;
//    }
//    ImGui::End();
//
//    //ImGui::SetNextWindowPos(ImVec2(SCR_WIDTH - 300, 10), ImGuiCond_Always);
//    ImGui::Begin("Point Query");
//    ImGui::SetWindowSize(ImVec2(250, 120));
//    // Input text field for the ID
//    ImGui::PushItemWidth(100);
//    ImGui::InputText("Enter ID", inputBuffer, sizeof(inputBuffer));
//    ImGui::PopItemWidth();
//
//    // Button to submit query
//    if (ImGui::Button("Query")) {
//        if (inputBuffer[0] != '\0') { // Check if inputBuffer is not empty
//            queryID = std::stoi(inputBuffer);
//            validID = queryID > 0 && queryID <= currentChunk.unit_count;
//        }
//        else {
//
//            validID = false;
//        }
//
//        if (validID) {
//           
//            std::cout << "Query ID: " << queryID << std::endl;
//            
//            VertexPoint point = calVertexPoint(currentChunk.vertices[queryID - 1], nextChunk.vertices[queryID - 1], currentDelta / renderTime, true);
//            glm::vec3 pos = point.position;
//            pos.x = pos.x;
//            pos.y = pos.y;
//
//            // Calculate grid location
//            int gridX = static_cast<int>((pos.x - leftBorder) / 0.002f); // Assumes grid size of 1.0
//            int gridY = static_cast<int>((pos.y - bottomBorder) / 0.002f);
//
//            positionInfo = "Position: (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")";
//            gridInfo = "Grid: (" + std::to_string(gridX) + ", " + std::to_string(gridY) + ")";
//            // Move camera to the point
//
//            //cout << pos.x << " " << pos.y << " " << pos.z << endl;
//
//            camera.MoveToPosition(pos, rig - lef, top - bottom, leftBorder, bottomBorder);
//        }
//        else {
//            positionInfo = "Invalid ID!";
//            gridInfo = "";
//        }
//    }
//
//    // Display position and grid information
//    ImGui::Text("%s", positionInfo.c_str());
//    ImGui::Text("%s", gridInfo.c_str());
//
//    ImGui::End();
//}
//
//void Render::renderMap(const Shader& shader, unsigned int VAO)
//{
//    // bind textures on corresponding texture units
//    glBindTexture(GL_TEXTURE_2D, textureId);
//    
//    // draw map
//    // -------------------------------------------------------------------------------
//    mapShader.use();
//
//    //lef = -1.0f, rig = 1.0f;
//    //bottom = -float(SCR_HEIGHT) / float(SCR_WIDTH), top = float(SCR_HEIGHT) / float(SCR_WIDTH);
//
//    /*lef = -1.0f, rig = 1.0f;
//    bottom = -float(SCR_HEIGHT) / float(SCR_WIDTH), top = float(SCR_HEIGHT) / float(SCR_WIDTH);*/
//    glm::mat4 projection = glm::ortho(lef, rig, bottom, top, near, far);
//    //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//    // camera/view transformation
//    glm::mat4 view = camera.GetViewMatrix();
//    glm::mat4 model = glm::mat4(1.0f);
//
//    mapShader.setMat4("projection", projection);
//    mapShader.setMat4("view", view);
//    mapShader.setMat4("model", model);
//
//
//
//
//    glBindVertexArray(mapVAO);
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//
//}
//
//void Render::renderEntities(std::vector<VertexPoint>& vertices)
//{
//    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
//    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexPoint) * vertices.size(), vertices.data());
//
//    std::cout << "Instance count: " << vertices.size() << std::endl;
//
//    glm::mat4 model = glm::mat4(1.0f);
//    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
//    glm::mat4 projection = glm::ortho(lef, rig, bottom, top, near, far);
//    glm::mat4 view = camera.GetViewMatrix();
//    //projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
//    entityShader.use();
//    entityShader.setMat4("model", model);
//    entityShader.setMat4("projection", projection);
//    entityShader.setMat4("view", view);
//    entityShader.setInt("texture_diffuse1", 0);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, rock.textures_loaded[0].id);
//    for (unsigned int i = 0; i < rock.meshes.size(); i++)
//    {
//        glBindVertexArray(rock.meshes[i].VAO);
//        glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(rock.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, vertices.size());
//        glBindVertexArray(0);
//    }
//}
//
//void Render::renderGrid(const Shader& shader, unsigned int VAO)
//{
//    gridShader.use();
//    glm::mat4 model = glm::mat4(1.0f);
//   
//    glm::mat4 projection = glm::ortho(lef, rig, bottom, top, near, far);
//    glm::mat4 view = camera.GetViewMatrix();
//    gridShader.setMat4("projection", projection);
//    gridShader.setMat4("view", view);
//    gridShader.setFloat("transparency", transparency);
//
//    glBindVertexArray(gridVAO);
//    glDrawArrays(GL_LINES, 0, gridVertices.size() / 3);
//}
//
//void Render::cleanup()
//{
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
//
//    // optional: de-allocate all resources once they've outlived their purpose:
//    // ------------------------------------------------------------------------
//    glDeleteVertexArrays(2, VAO);
//    glDeleteBuffers(2, VBO);
//    glDeleteBuffers(2, EBO);  // ??EBO????
//
//    // Cleanup
//    glDeleteVertexArrays(1, &gridVAO);
//    glDeleteBuffers(1, &gridVBO);
//    // glfw: terminate, clearing all previously allocated GLFW resources.
//    // ------------------------------------------------------------------
//    glfwTerminate();
//}
//
//void Render::renderUI()
//{
//}
//
//DataChunk Render::getNextChunk(Data& data, bool useNewDataFetch, std::queue<DataChunk>& dataChunkBuffer, std::mutex& dataChunkMutex)
//{
//    return DataChunk();
//}
//
//void Render::updateInstances(std::vector<VertexPoint>& vertices, float base_area)
//{
//
//    float current_area = (rig - lef) * (top - bottom);
//    float scale_factor = current_area / base_area;
//    float skip_probability = (scale_factor - 1.0f) * 0.5f;  // ??????
//    if (skip_probability < 0.0f) skip_probability = 0.0f;
//    if (skip_probability > 0.95f) skip_probability = 0.95;  // ???????70%
//
//    //std::cout << "Skip Probability: " << skip_probability << std::endl;
//    int flagIndex = 0;
//
//    for (int i = 0; i < currentChunk.unit_count; i++) {
//        if (i == queryID - 1) {  // ?????
//            VertexPoint point = calVertexPoint(currentChunk.vertices[i], nextChunk.vertices[i], currentDelta / renderTime, true);
//            vertices.push_back(point);
//            //glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), point.position);
//
//            /* flagIndex = vertices.size() - 1;
//            cout << "vertices " << vertices.size() << " " << vertices[vertices.size() - 1].position.x << " " <<  vertices[vertices.size() - 1].position.y <<" " << i + 1 << " " << vertices[vertices.size() - 1].status << endl;*/
//        }
//        else if (probabilities[i] > skip_probability) {  // ??????????
//            VertexPoint point = calVertexPoint(currentChunk.vertices[i], nextChunk.vertices[i], currentDelta / renderTime, false);
//            vertices.push_back(point);
//            //glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), point.position);
//        }
//
//    }
//
//    /*std::vector<int> indexLines = currentDelta < 0.5f
//        ? std::vector<int>(currentChunk.indices, currentChunk.indices + currentChunk.unit_count)
//        : std::vector<int>(nextChunk.indices, nextChunk.indices + nextChunk.unit_count);*/
//}
//
//
//void Render::processInput(GLFWwindow* window)
//{
//    int currentKeyStateE = glfwGetKey(window, GLFW_KEY_E);
//    if (currentKeyStateE == GLFW_PRESS && previousKeyStateE == GLFW_RELEASE) {
//        // ?? E ????
//        if (renderSpeed < 3.0f) {
//            renderSpeed += 0.5f;
//            std::cout << "Render Speed: " << renderSpeed << "x" << std::endl;
//        }
//    }
//    // ??????
//    previousKeyStateE = currentKeyStateE;
//
//    // ?? Q ?
//    int currentKeyStateQ = glfwGetKey(window, GLFW_KEY_Q);
//    if (currentKeyStateQ == GLFW_PRESS && previousKeyStateQ == GLFW_RELEASE) {
//        // ?? Q ????
//        if (renderSpeed > 0.5f) {
//            renderSpeed -= 0.5f;
//            std::cout << "Render Speed: " << renderSpeed << "x" << std::endl;
//        }
//    }
//    // ??????
//    previousKeyStateQ = currentKeyStateQ;
//
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        camera.ProcessKeyboard(FORWARD, deltaTime, topBorder, top);
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        camera.ProcessKeyboard(BACKWARD, deltaTime, bottomBorder, bottom);
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        camera.ProcessKeyboard(LEFT, deltaTime, leftBorder, lef);
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        camera.ProcessKeyboard(RIGHT, deltaTime, rightBorder, rig);
//}
//
//void Render::handleQuery(int queryID, const DataChunk& currentChunk, const DataChunk& nextChunk, float currentDelta, float renderTime, std::queue<int>& queryIdBuffer, std::mutex& queryIdMutex, std::string& positionInfo, std::string& gridInfo)
//{
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    // make sure the viewport matches the new window dimensions; note that width and 
//    // height will be significantly larger than specified on retina displays.
//
//    glViewport(0, 0, width, height);
//    //cout << "width: " << width << "height: " << height << endl;
//    bottom = float(height) / float(width) * lef;
//    top = float(height) / float(width) * rig;
//}
//
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//    float xpos = static_cast<float>(xposIn);
//    float ypos = static_cast<float>(yposIn);
//
//    if (firstMouse)
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//    lastX = xpos;
//    lastY = ypos;
//
//    camera.ProcessMouseMovement(xoffset, yoffset);
//}
//
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    float zoomFactor = 0.1f;
//    float zoomAmount = yoffset * zoomFactor;
//
//    // Calculate the aspect ratio of the viewport
//    float aspectRatio = static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT);
//
//    // Adjust the orthographic projection bounds
//    float width = rig - lef;
//    float height = top - bottom;
//
//    // Update the bounds based on the zoom amount
//    width -= zoomAmount;
//    height -= zoomAmount / aspectRatio;
//
//
//    // Ensure width and height remain positive
//    if (width <= 0.0f) width = 0.1f;
//    if (height <= 0.0f) height = 0.1f;
//
//    // Ensure height is within minHeight and maxHeight
//    if (height > maxHeight)
//    {
//        height = maxHeight;
//        return;
//    }
//    if (height < minHeight)
//    {
//        height = minHeight;
//        return;
//    }
//
//    // Maintain aspect ratio
//    float centerX = (lef + rig) / 2.0f;
//    float centerY = (bottom + top) / 2.0f;
//
//    lef = centerX - width / 2.0f;
//    rig = centerX + width / 2.0f;
//    bottom = centerY - height / 2.0f;
//    top = centerY + height / 2.0f;
//
//    // Ensure the view frustum does not exceed the map borders
//    if (lef < leftBorder) lef = leftBorder;
//    if (rig > rightBorder) rig = rightBorder;
//    if (bottom < bottomBorder) bottom = bottomBorder;
//    if (top > topBorder) top = topBorder;
//}
//
//VertexPoint calVertexPoint(VertexPoint v1, VertexPoint v2, float p, bool highLight)
//{
//    glm::vec3 v = v1.position + (v2.position - v1.position) * p;
//
//    v = v * 4.0f;
//
//
//    float scale = abs(v.y / earthRadius);
//    if (scale > 1.0f) {
//        v.z = -1.0f;
//    }
//
//
//    v.x = sqrt(earthRadius * earthRadius - v.y * v.y) * (v.x > 0.f ? 1.f : -1.f) * v.x / earthRadius;
//
//
//    float distance = glm::length(glm::vec2(v.x, v.y));
//    if (distance > earthRadius) {
//
//
//        v.z = -1.0f;
//    }
//    else {
//        v.z = 0.0f;
//    }
//
//
//
//
//    VertexPoint vertex(v, v1.color, v1.camps, v1.status);
//
//    if (highLight) {
//        vertex.padding = 1;
//    }
//
//    return vertex;
//}
