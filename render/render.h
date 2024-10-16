#ifndef RENDER_H
#define RENDER_H
#include <queue>
#include <mutex>



class Render {
public:
    Render();
    void render(std::queue<int>& queryIdBuffer, std::mutex& queryIdMutex, std::queue<DataChunk>& dataChunkBuffer, std::mutex& dataChunkMutex);
    void freeDataChunk(DataChunk dataChunk);

private:
    
};

#endif