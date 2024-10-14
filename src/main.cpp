#include <iostream>
#include <ctime>
#include <cstdlib>
#include <map>
#include <vector>
#include <time.h>
#include <algorithm>
#include "../include/Utils.h"
#include "../render/render.h"
#include <numeric>
#include <thread>

using namespace std;

int main() {
  std::queue<DataChunk> dataChunkBuffer; // 缓冲区
  std::mutex dataChunkMutex; // 互斥量

  // 生成渲染线程
  Render render;
  std::thread dataThread(&Render::render, std::ref(render), std::ref(dataChunkBuffer), std::ref(dataChunkMutex));

  ofstream logFile("log.txt", std::ios::trunc);
  if (!logFile.is_open())
    std::cerr << "Unable to open log file!" << std::endl;

  vector<int> units;
  map<int, shared_ptr<BaseUnit>> all_units;
  map<int, MapRegion> all_regions;
  // 不同类的个数需要根据任务书分配
  units.push_back(1);                   // 基站
  units.push_back(1000);      // 飞行器
  units.push_back(1000);    // 导弹
  units.push_back(2000);      // 坦克
  units.push_back(5000);  // 士兵

  int unit_num = accumulate(units.begin(), units.end(), 0);
  //  地图格点的长和宽
  int map_size_x = 10000;
  int map_size_y = 10000;
  int map_size_z = 1000;

  // 开始生成对象
  printf("Start initial...\n");
  int camp0_position[4] = { 7200, 4800, 1200, 550 };
  int camp1_position[4] = { 5800, 5900, 2800, 800 };
  InitAllUnits(units, all_units, map_size_x, map_size_y, map_size_z, 0, unit_num, camp0_position, camp1_position); // 分别初始化两个阵营
  InitAllUnits(units, all_units, map_size_x, map_size_y, map_size_z, 1, unit_num, camp1_position, camp0_position);
  
  unit_num *= 2;

  int time_length = 500;
  int time_slice = 1;
  clock_t start, end, allend; 
  
  double *positions, *target_positions, *speeds, *directions, *distances;
  double *g_positions, *g_target_positions, *g_speeds, *g_directions, *g_distances, *g_init_positions;

  double *attack_radius, *g_attack_radius;
  int *target_ids, *weapon_nums, *status, *unit_class;
  int *g_target_ids, *g_weapon_nums, *g_status, *g_unit_class;
  Vertex *vertices, *g_vertices; // for DataChunk
  int *indices, *g_indices; // for DataChunk

  cudaMalloc(&g_positions,        sizeof(double) * 3 * unit_num);
  cudaMalloc(&g_target_positions, sizeof(double) * 3 * unit_num);
  cudaMalloc(&g_speeds,           sizeof(double) * unit_num);
  cudaMalloc(&g_directions,       sizeof(double) * 3 * unit_num);
  cudaMalloc(&g_distances,        sizeof(double) * 3 * unit_num);
  cudaMalloc(&g_init_positions,   sizeof(double) * 3 * unit_num);

  cudaMalloc(&g_target_ids,       sizeof(int) * unit_num);
  cudaMalloc(&g_weapon_nums,      sizeof(int) * unit_num);
  cudaMalloc(&g_status,           sizeof(int) * unit_num);
  cudaMalloc(&g_attack_radius,    sizeof(double) * unit_num);
  cudaMalloc(&g_unit_class,       sizeof(int) * unit_num);

  cudaMalloc(&g_vertices,         sizeof(Vertex) * unit_num);
  cudaMalloc(&g_indices,          sizeof(int) * unit_num);

  positions            = new double[unit_num * 3];
  target_positions     = new double[unit_num * 3];
  speeds               = new double[unit_num];
  directions           = new double[unit_num * 3];
  distances            = new double[unit_num * 3];

  target_ids           = new int[unit_num];
  weapon_nums          = new int[unit_num];
  status               = new int[unit_num];
  attack_radius        = new double[unit_num];
  unit_class           = new int[unit_num];

  vertices             = new Vertex[unit_num];
  indices              = new int[unit_num];

  // 初始化
  for (unsigned i = 0; i < all_units.size(); i++) {
    auto unit_i = all_units.find(i)->second;
    positions[i*3] = unit_i->getPositionX();
    positions[i*3 + 1] = unit_i->getPositionY();
    positions[i*3 + 2] = unit_i->getPositionZ();
    if (unit_i->getTargetId() != 0) {
      auto unit_target = all_units.find(unit_i->getTargetId())->second;
      target_positions[i*3]   = unit_target->getPositionX();
      target_positions[i*3+1] = unit_target->getPositionY();
      target_positions[i*3+2] = unit_target->getPositionZ();
    } else {
      target_positions[i*3]   = unit_i->getTargetPositionX();
      target_positions[i*3+1] = unit_i->getTargetPositionY();
      target_positions[i*3+2] = unit_i->getTargetPositionZ();
    }
    speeds[i] = unit_i->getSpeed();
    directions[i*3] = unit_i->getDirectionX();
    directions[i*3 + 1] = unit_i->getDirectionY();
    directions[i*3 + 2] = unit_i->getDirectionZ();
    distances[i*3] = 0; // 这里先初始化成0
    distances[i*3 + 1] = 0;
    distances[i*3 + 2] = 0;

    target_ids[i] = unit_i->getTargetId();
    weapon_nums[i] = unit_i->getWeaponNum();
    status[i] = unit_i->getStatus();
    attack_radius[i] = unit_i->getAttackRadius();

    indices[i] = i;
    vertices[i] = Vertex(glm::vec3(unit_i->getPositionX(), unit_i->getPositionY(), unit_i->getPositionZ()), 
                         glm::vec3(0, 0, 0), unit_i->getStatus(), unit_i->getCamp());
  }
  int count = 0;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < units.size(); j++) {
      for (int k = 0; k < units[j]; k++) {
        if (j >= 5) unit_class[count] = j - 5;
        else unit_class[count] = j;
        count++;
      }
    }
  }

  cudaMemcpy(g_positions, positions,               sizeof(double) * 3 * unit_num, cudaMemcpyHostToDevice);
  cudaMemcpy(g_target_positions, target_positions, sizeof(double) * 3 * unit_num, cudaMemcpyHostToDevice);
  cudaMemcpy(g_speeds, speeds,                     sizeof(double) * unit_num, cudaMemcpyHostToDevice);
  cudaMemcpy(g_directions, directions,             sizeof(double) * 3 * unit_num, cudaMemcpyHostToDevice);
  cudaMemcpy(g_distances, distances,               sizeof(double) * 3 * unit_num, cudaMemcpyHostToDevice);
  cudaMemcpy(g_init_positions, positions,          sizeof(double) * 3 * unit_num, cudaMemcpyHostToDevice);

  cudaMemcpy(g_target_ids, target_ids,              sizeof(int) * unit_num, cudaMemcpyHostToDevice);
  cudaMemcpy(g_weapon_nums, weapon_nums,            sizeof(int) * unit_num, cudaMemcpyHostToDevice);
  cudaMemcpy(g_status, status,                      sizeof(int) * unit_num, cudaMemcpyHostToDevice);
  cudaMemcpy(g_attack_radius, attack_radius,        sizeof(double) * unit_num, cudaMemcpyHostToDevice);
  cudaMemcpy(g_unit_class, unit_class,              sizeof(int) * unit_num, cudaMemcpyHostToDevice);

  cudaMemcpy(g_vertices, vertices,                  sizeof(Vertex) * unit_num, cudaMemcpyHostToDevice);
  cudaMemcpy(g_indices, indices,                    sizeof(int) * unit_num, cudaMemcpyHostToDevice);

  start = clock();

  if (cudaCalculateDistances(positions, target_positions, distances, g_positions, g_target_positions, g_distances, unit_num) != 1) {
    printf("Cuda Cal Distances error.\n");
    return 0;
  }
  
  all_units.find(0)->second->Update(time_slice, all_units, positions, target_positions, directions, distances, target_ids, status);
  all_units.find(unit_num/2)->second->Update(time_slice, all_units, positions, target_positions, directions, distances, target_ids, status);

  if (cudaUnitUpdate(g_positions, g_target_positions, g_directions, g_distances,
                     g_target_ids, g_init_positions, g_weapon_nums, g_status, g_attack_radius, g_unit_class,
                     target_ids, directions, weapon_nums, status, target_positions, unit_num) != 1) {
    printf("Cuda Unit Update error.\n");
    return 0;
  }

  if (cudaUpdatePositions(directions, positions, speeds, vertices, indices, g_directions, g_positions, g_speeds, g_vertices, g_indices, g_status, g_unit_class,
                          map_size_x, map_size_y, map_size_z, time_slice, unit_num) != 1) {
    printf("Cuda Update Positions error.\n");
    return 0;
  }

  LogPrint(logFile, 0, status, unit_class, unit_num);

  ProduceData(dataChunkBuffer, dataChunkMutex, vertices, indices, unit_num);

  for (int i = 1; i < time_length; ++i) {
    printf(">>>>>>>>>>>>>>>>%d\n", i);

    if (cudaCalculateDistances(positions, target_positions, distances, g_positions, g_target_positions, g_distances, unit_num) != 1) {
      printf("Cuda Cal Distances error.\n");
      return 0;
    }
    
    all_units.find(0)->second->Update(time_slice, all_units, positions, target_positions, directions, distances, target_ids, status);
    all_units.find(unit_num/2)->second->Update(time_slice, all_units, positions, target_positions, directions, distances, target_ids, status);

    if (cudaUnitUpdate(g_positions, g_target_positions, g_directions, g_distances,
                       g_target_ids, g_init_positions, g_weapon_nums, g_status, g_attack_radius, g_unit_class,
                       target_ids, directions, weapon_nums, status, target_positions, unit_num) != 1) {
      printf("Cuda Unit Update error.\n");
      return 0;
    }

    if (cudaUpdatePositions(directions, positions, speeds, vertices, indices, g_directions, g_positions, g_speeds, g_vertices, g_indices, g_status, g_unit_class,
                          map_size_x, map_size_y, map_size_z, time_slice, unit_num) != 1) {
      printf("Cuda Update Positions error.\n");
      return 0;
    }

    LogPrint(logFile, i, status, unit_class, unit_num);

    ProduceData(dataChunkBuffer, dataChunkMutex, vertices, indices, unit_num);
  }
  end = clock();
  dataThread.join();  // 等待前端运行结束
  allend = clock();
  printf("Run time = %fs, All time = %fs\n", (double)(end - start) / CLOCKS_PER_SEC, (double)(allend - start) / CLOCKS_PER_SEC);
  logFile.close(); // 关闭文件

  delete[] positions;
  delete[] target_positions;
  delete[] speeds;
  delete[] directions;
  delete[] distances;
  cudaFree(g_positions);
  cudaFree(g_target_positions);
  cudaFree(g_speeds);
  cudaFree(g_directions);
  cudaFree(g_distances);
  cudaFree(g_init_positions);
  delete[] target_ids;
  delete[] weapon_nums;
  delete[] status;
  delete[] attack_radius;
  delete[] unit_class;
  cudaFree(g_target_ids);
  cudaFree(g_weapon_nums);
  cudaFree(g_status);
  cudaFree(g_attack_radius);
  cudaFree(g_unit_class);
  delete[] vertices;
  delete[] indices;
  cudaFree(g_vertices);
  cudaFree(g_indices);

  return 0;
}