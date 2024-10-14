#ifndef DATA_PRODUCER_H
#define DATA_PRODUCER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <json/json.h>
#include <fstream>

#include <direct.h>

#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <map>
#include <GLFW/glfw3.h>

using namespace std;

// Point
struct Vertex {
	// position
	glm::vec3 position;
	// normal
	glm::vec3 color;

	int status;
	int camps;
	int padding;


	Vertex(const glm::vec3& position, const glm::vec3& color, int camps, int status)
		: position(position), color(color), camps(camps), status(status),padding(0) {}

	Vertex()
		: position(0.0f), color(0.0f), status(0), camps(0), padding(0) {}
};

// Line
struct Line {

	Vertex vertex1;

	Vertex vertex2;

	Line(const Vertex& vertex1, const Vertex& vertex2)
		: vertex1(vertex1), vertex2(vertex2) {}
};

// Point and line
//struct DataChunk {
//	vector<Vertex> vertices;
//	vector<int> indices;
//};


struct DataChunk {
	Vertex* vertices;
	int* indices;
	int unit_count;
};


// Data Parser
class Data {
private:
	
	int index = 0; //file parse index
	int readIndex = 0; // data get index
	queue<DataChunk> dataQueue; 
	mutex queueMutex; // dataMap mutex
	mutex indexMutex; // index mutex
	const int QUEUE_MAX_SIZE = 5;
	condition_variable full;
	condition_variable empty;
	map<int, DataChunk> dataMap; //data map, size = QUEUE_MAX_SIZE
public:
	void PrintDataChunk(DataChunk dataChunk) {
		Vertex* vertices = dataChunk.vertices;
		for (int i = 0; i < dataChunk.unit_count; i++) {
			printf("camp: %d, status: %d, positions: %f\t%f\t%f, color: %f\t%f\t%f\n", vertices[i].camps, vertices[i].status, vertices[i].position.x, vertices[i].position.y, vertices[i].position.z,
																			vertices[i].color.x, vertices[i].color.y, vertices[i].color.z);
		}
	}
	// Parse data from json file
	void readDataFromJson()
	{
		string RenderPath = std::string(PROJECT_SOURCE_DIR) + "/render";
		while (true) {
			vector<Vertex> vertices;
			vector<int> indices;
			
			unique_lock<std::mutex> indexlock(indexMutex);
			int localIndex = this->index;
			this->index = (index + 1) % 50;
			std::string fileName = RenderPath + "/data1/unit_info_" + std::to_string(localIndex) + ".json";
			indexlock.unlock();
			
			// parse json
			Json::Reader reader;
			Json::Value root;
			ifstream srcFile(fileName, ios::binary);
			//char* buffer; 
			//if ((buffer = getcwd(NULL, 0)) == NULL)
			//{
			//	perror("getcwd error");
			//}
			//else
			//{
			//	printf("path:%s\n", buffer);
			//	free(buffer);
			//}

			if (!srcFile.is_open())
			{

				cout << "Fail to open " + fileName << endl;
				return;
			}
			else
			{
				//cout << "Get " + fileName << endl;

			}
			//vector<float> tmpVertices;
			

			float time1 = static_cast<float>(glfwGetTime());
			float time2 = 0.0f;
			if (reader.parse(srcFile, root))
			{

				time2 = static_cast<float>(glfwGetTime());
				//cout << "parse file time : " << time2 - time1 << endl;

				time1 = static_cast<float>(glfwGetTime());
				float scale = 2000.0;
				for (int i = 0; i < root["entity"].size(); i++) {
					int id = root["entity"][i]["id"].asInt();
					float posX = (root["entity"][i]["posX"].asDouble() - 5000.0) / scale;
					float posY = (root["entity"][i]["posY"].asDouble() - 5000.0) / scale;
					float posZ = root["entity"][i]["posZ"].asDouble();
					string type = root["entity"][i]["type"].asString();
					glm::vec3 pos = glm::vec3(posX, posY, posZ);
					glm::vec3 color = type == "Unit" ? glm::vec3(1, 0, 0) : glm::vec3(0, 1, 0);
					Vertex vertex = Vertex(pos, color, 0, 0);
					vertices.push_back(vertex);
					
			
				}

				time2 = static_cast<float>(glfwGetTime());
				//cout << "get points time : " << time2 - time1 << endl;


				time1 = static_cast<float>(glfwGetTime());
				
				for (int i = 0; i < root["relation"].size(); i++) {
					int id1 = root["relation"][i]["entity1"].asInt();
					int id2 = root["relation"][i]["entity2"].asInt();
					indices.push_back(id1);
					indices.push_back(id2);
					
				}
				time2 = static_cast<float>(glfwGetTime());
				//cout << "get lines time : " << time2 - time1 << endl;

			}
			srcFile.close();

	/*		DataChunk chunk;

			chunk.vertices = vertices;
			chunk.indices = indices;*/

			DataChunk chunk;
			chunk.unit_count = vertices.size();
			chunk.vertices = new Vertex[chunk.unit_count];
			chunk.indices = new int[indices.size()];
			for (int i = 0; i < chunk.unit_count; i++) {
				chunk.vertices[i] = vertices[i];
			}

			/*for (int i = 0; i < indices.size(); i++) {
				chunk.indices[i] = indices[i];
			}*/


			// insert dataChunk to datamap
			unique_lock<std::mutex> lock(queueMutex);
			/*full.wait(lock, [this] {
				if (dataMap.size() >= QUEUE_MAX_SIZE) {
					return false;
				}
				return true;
				});*/
			full.wait(lock, [this, localIndex] {
				if (localIndex - readIndex > 10 || (readIndex > localIndex && readIndex - localIndex < 40)) {
					return false;
				}
				return true;
				});
			dataMap.insert(std::pair<int, DataChunk>(localIndex, chunk));
			//cout << "insert chunk indices : " << dataMap[localIndex].indices.size() << "chunk vertices : " << chunk.vertices.size() << " localIndex:"<<  localIndex << endl;
			//cout << fileName << " size: " << dataMap.size() << endl;
			empty.notify_one();
			
			
		}
	}
		


	// get data from dataMap and notify it to parse json file
	DataChunk getDataChunk() {
		DataChunk chunk;
		unique_lock<std::mutex> lock(queueMutex);
		empty.wait(lock, [this] {
			if (dataMap.count(readIndex) == 0) {
				return false;
			}
			return true;
			});

		//cout << "readIndex : " << readIndex << "size: " << dataMap.size() << endl;
		chunk = dataMap[readIndex];
		//cout << "get chunk indices : " << chunk.indices.size() << "chunk vertices : " << chunk.vertices.size() << endl;
		dataMap.erase(readIndex);
		readIndex = (readIndex + 1) % 50;
		full.notify_one();

		return chunk;

	}

	DataChunk getDataChunk(std::queue<DataChunk>& dataChunkBuffer, std::mutex& dataChunkMutex) {
		DataChunk chunk;
		while (true) {
			//std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 10));
			unique_lock<std::mutex> lock(dataChunkMutex);
			if (dataChunkBuffer.size()) {
				// chunk = dataChunkBuffer.front();
				chunk = std::move(dataChunkBuffer.front());
				dataChunkBuffer.pop();
				static int count = 0;
				std::cout << "Consumed a DataChunk: " << count++ << std::endl;
				//PrintDataChunk(chunk);
				break;
			}
		}
		
		return chunk;
	}
	
	
	void getDataAsyc() {
		std::thread dataThread(&Data::readDataFromJson, this);
		dataThread.detach();
	}

};





#endif