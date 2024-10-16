#ifndef UTILS_H
#define UTILS_H

#include "BaseStationUnit.h"
#include "PlaneUnit.h"
#include "MissleUnit.h"
#include "TankUnit.h"
#include "SoldierUnit.h"
#include "MapRegion.h"
#include "CudaFunc.cuh"
//#include "/data/luyao/luyao/json/include/nlohmann/json.hpp"
#include <cuda_runtime.h> 
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <queue>
#include <mutex>
#include <algorithm>
#include <time.h>
#include <fstream>
#include <windows.h>
#include <psapi.h>

using namespace std;

void GetRand(int num, int min, int max, vector<int>& rand_data);
void InitAllUnits(vector<int> units, map<int, shared_ptr<BaseUnit>>& all_units, int map_size_x, int map_size_y, int map_size_z, int camp, int unit_num,
					int* my_position, int* enemy_position);
void PrintAllUnit(map<int, shared_ptr<BaseUnit>> all_units, int time_stamp);
void PrintAllUnit(map<int, shared_ptr<BaseUnit>> all_units, int time_stamp, double *positions, double *directions, int *status);
void SaveAllUnitInfo(map<int, shared_ptr<BaseUnit>> all_units, int time_stamp);
void PrintUnitRelated(map<int, shared_ptr<BaseUnit>> all_units);
void InitAllRegions(map<int, MapRegion>& all_regions, int map_size_row, int map_size_col, int radius);
void PrintRegions(map<int, MapRegion> all_regions);
map<int, map<int, double>> GetChangeTime(int rand_kind, int time_length, int num, map<int, shared_ptr<BaseUnit>> all_units);
void PrintChangeTime(map<int, map<int, double>> change_times);
double CalculateDistance(double x1, double y1, double z1, double x2, double y2, double z2);
void GetMapRegion(shared_ptr<BaseUnit>& unit, map<int, MapRegion>& map_regions);
void RefreshMapRegionUnits(map<int, shared_ptr<BaseUnit>>& all_units, map<int, MapRegion>& all_regions);
void PrintMapRegionUnits(map<int, MapRegion> all_regions);
void ChangeSpeed(BaseUnit& unit, double time_slice, double new_speed, double directionX, double directionY, double directionZ, double map_sizeX, double map_sizeY, double map_sizeZ);
void InitRelatedUnits(map<int, shared_ptr<BaseUnit>>& all_units);
void ClearRelatedObjects(vector<int> cur_ids, map<int, shared_ptr<BaseUnit>>& all_units);
void InitRelatedRegions(map<int, MapRegion>& all_regions, int radius);
void RefreshUnitsRelated(vector<int> cur_ids, map<int, shared_ptr<BaseUnit>>& all_units, map<int, MapRegion> all_regions);
bool IsConnected(shared_ptr<BaseUnit> unit_fir, shared_ptr<BaseUnit> unit_sec);
void ProduceData(queue<DataChunk>& dataChunkBuffer, mutex& dataChunkMutex, Vertex *vertices, int *indices, int unit_num);
void Log1Print(ofstream &log_file, double *positions, int unit_num);
void Log2Print(ofstream &log_file, double *positions, double *directions, int *status, int unit_num);
void Log3Print(ofstream &log_file, int time_stamp, int *status, int *unit_class, int unit_num, double run_time);
void Log5Print(ofstream &log_file, int time_stamp);
#endif // UTILS_H
