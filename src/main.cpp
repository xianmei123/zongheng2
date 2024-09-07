#include <iostream>
#include <ctime>
#include <cstdlib>
#include <map>
#include <vector>
#include <time.h>
#include <algorithm>
#include <cuda_runtime.h> 
#include "../include/Utils.h"
#include "../include/CudaFunc.cuh"
#include "../render/render.h"
#include <numeric>
#include <thread>

using namespace std;

//void PrintDataChunk(DataChunk dataChunk) {
//  Vertex *vertices = dataChunk.vertices;
//  int *indices = dataChunk.indices;
//  for (int i = 0; i < dataChunk.unit_count; i++) {
//    printf("indices: %d, status: %d, positions: %f\t%f\t%f\n", indices[i], vertices[i].status, vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
//  }
//}

void produce_data(std::queue<DataChunk>& dataChunkBuffer, std::mutex& dataChunkMutex, Vertex *vertices, int *indices, int unit_num) {
  Vertex* tmp_vertices = new Vertex[unit_num];
  int* tmp_indices = new int[unit_num];
  memcpy(tmp_vertices, vertices, sizeof(Vertex) * unit_num);
  memcpy(tmp_indices, indices, sizeof(int) * unit_num);
  DataChunk newDataChunk = { tmp_vertices, tmp_indices, unit_num };
  static int count = 0;
  std::unique_lock<std::mutex> lock(dataChunkMutex); // 锁定互斥量
  dataChunkBuffer.push(newDataChunk);
  std::cout << "Produced a DataChunk: " << count++ << std::endl;
}


int main() {
  // 所有类的总个数
  // int unit_num = 300;
  // int unit_kind = 4;
  std::queue<DataChunk> dataChunkBuffer; // 缓冲区
  std::mutex dataChunkMutex; // 互斥量

  // 生成渲染线程
  Render render;
  std::thread dataThread(&Render::render, std::ref(render), std::ref(dataChunkBuffer), std::ref(dataChunkMutex));
  //dataThread.detach();
  
  vector<int> units;
  map<int, shared_ptr<BaseUnit>> all_units;
  map<int, MapRegion> all_regions;
  // 随机生成四个类的比例，根据比例生成类的对象，分别存在 TODO：不同类的个数需要根据任务书分配
  srand(time(0));
  // vector<int> rand_data;
  // GetRand(unit_kind, 1, 10, rand_data);
  // for (int i = 0; i < unit_kind; ++i) {
  //   units.push_back((int)(unit_num * (float)rand_data[i] / rand_data.back()));
  // }
  // 
  units.push_back(1);                   // 基站
  units.push_back(3000);      // 飞行器
  units.push_back(3000);    // 导弹
  units.push_back(10000);      // 坦克
  units.push_back(34000);  // 士兵

  //units.push_back(1);                  // 基站
  //units.push_back(1);                  // 飞行器
  //units.push_back(10);                 // 导弹
  //units.push_back(10);                  // 坦克
  //units.push_back(10);                 // 士兵

  int unit_num = accumulate(units.begin(), units.end(), 0);
  //  地图格点的长和宽
  int map_size_x = 10000;
  int map_size_y = 10000;
  int map_size_z = 1000;
  int radius = 100;
  // 开始生成对象
  printf("Start initial...\n");
  InitAllRegions(all_regions, map_size_x, map_size_y, radius);  
  InitRelatedRegions(all_regions, radius);
  // PrintRegions(all_regions);
  InitAllUnits(units, all_units, map_size_x, map_size_y, map_size_z, 0, unit_num); // 分别初始化两个阵营
  InitAllUnits(units, all_units, map_size_x, map_size_y, map_size_z, 1, unit_num);
  // InitRelatedUnits(all_units);
  // PrintAllUnit(all_units, 0);
  // PrintUnitRelated(all_units);
  // SaveAllUnitInfo(all_units, 0);   // 保存初始化的状态
  // RefreshMapRegionUnits(all_units, all_regions);
  // PrintMapRegionUnits(all_regions);

  unit_num *= 2;
  // 得到速度变化时刻表
  int rand_kind = 1;
  int time_length = 50;
  int num = 100;
  int time_slice = 1;
  clock_t start, end, allend; 
  
  double *positions, *target_positions, *speeds, *directions, *distances;
  // double *g_positions, *g_target_positions, *g_speeds, *g_directions, *g_distances;
  // 全局或静态变量，用于存储设备指针
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

  // static double *g_positions, *g_target_positions, *g_speeds, *g_directions, *g_distances;
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

  // printf("Init Begin\n");
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
  // printf("Init Over\n");

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

  // cudaVoidInit(
  //   g_positions, g_target_positions, g_speeds, g_directions, g_distances,
  //   positions, target_positions, speeds, directions, distances,
  //   unit_num);

  // for (int j = 0; j < unit_num; j++) {
  //     for (int k = 0; k < 3; k++) {
  //         printf("%f ", positions[j*3 + k]);
  //     }
  //     printf("\n");
  // }
  // PrintAllUnit(all_units, 0);

  if (cudaCalculateDistances(positions, target_positions, distances, g_positions, g_target_positions, g_distances, unit_num) != 1) {
    printf("Cuda Cal Distances error.\n");
    return 0;
  }
  
  // for (unsigned i = 0; i < all_units.size(); ++i) {
  //   auto unit_i = all_units.find(i)->second;
  //   unit_i->Update(time_slice, all_units, positions, target_positions, directions, distances);
  // }
  all_units.find(0)->second->Update(time_slice, all_units, positions, target_positions, directions, distances, target_ids);
  all_units.find(unit_num/2)->second->Update(time_slice, all_units, positions, target_positions, directions, distances, target_ids);

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

  produce_data(dataChunkBuffer, dataChunkMutex, vertices, indices, unit_num);

  //PrintDataChunk(dataChunk);
  
  //all_units.find(0)->second.move(time_slice);
  // map<int, map<int, double>> change_times = GetChangeTime(rand_kind, time_length, num, all_units);
  // PrintChangeTime(change_times);
  start = clock();
  for (int i = 1; i < time_length; ++i) {
    printf(">>>>>>>>>>>>>>>>");
    printf("%d\n", i);
    // PrintAllUnit(all_units, i);
    // RefreshMapRegionUnits(all_units, all_regions);
    // vector<int> cur_ids;
    // if (i % 5 == 0) {
    //   for (unsigned j = 0; j < all_units.size(); ++j) {
    //     if (all_units.find(j)->second->getPriority() != 4) {
    //       cur_ids.push_back(j);
    //     }
    //   }
    // } else if (i % 2 == 0) {
    //   for (unsigned j = 0; j < all_units.size(); ++j) {
    //     if (all_units.find(j)->second->getPriority() == 1 || all_units.find(j)->second->getPriority() == 2) {
    //       cur_ids.push_back(j);
    //     }
    //   }
    // } else {
    //   for (unsigned j = 0; j < all_units.size(); ++j) {
    //     if (all_units.find(j)->second->getPriority() == 1) {
    //       cur_ids.push_back(j);
    //     }
    //   }
    // }
    // RefreshUnitsRelated(cur_ids, all_units, all_regions);
    // PrintUnitRelated(all_units);
    // PrintAllUnit(all_units, i);
    // PrintAllUnit(all_units, i, positions, directions, status);
    // SaveAllUnitInfo(all_units, i);   // 保存每一时刻的状态
    // if (change_times.find(i) != change_times.end()) {
    //   auto units = change_times.find(i)->second;
    //   for (auto unit_id : units) {
    //     all_units.find(unit_id.first)->second->setSpeed(unit_id.second);
    //   }
    // }

    if (cudaCalculateDistances(positions, target_positions, distances, g_positions, g_target_positions, g_distances, unit_num) != 1) {
      printf("Cuda Cal Distances error.\n");
      return 0;
    }
    
    // for (unsigned j = 0; j < all_units.size(); ++j) {
    //   auto unit_j = all_units.find(j)->second;
    //   unit_j->Update(time_slice, all_units, positions, target_positions, directions, distances);
    // }
    all_units.find(0)->second->Update(time_slice, all_units, positions, target_positions, directions, distances, target_ids);
    all_units.find(unit_num/2)->second->Update(time_slice, all_units, positions, target_positions, directions, distances, target_ids);
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

    produce_data(dataChunkBuffer, dataChunkMutex, vertices, indices, unit_num);
    // PrintAllUnit(all_units, i, positions, directions, status);
    
  }
  end = clock();
  // PrintAllUnit(all_units, time_length, positions, directions, status);
  // printf("Run time = %fs, All time = %fs\n", (double)(end - start) / CLOCKS_PER_SEC);
  //std::thread dataThread(&Render::render, std::ref(render), std::ref(dataChunkBuffer), std::ref(dataChunkMutex));
  dataThread.join();  // 等待前端运行结束
  allend = clock();
  printf("Run time = %fs, All time = %fs\n", (double)(end - start) / CLOCKS_PER_SEC, (double)(allend - start) / CLOCKS_PER_SEC);
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