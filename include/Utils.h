#ifndef UTILS_H
#define UTILS_H

#include "BaseUnit.h"
#include "MapRegion.h"
#include "FlightUnit.h"
#include "GroundUnit.h"
#include "NonMovableUnit.h"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <algorithm>
#include <time.h>

using namespace std;

//设置两个实体距离小于多少时，二者可以建立联系
#define CONNECTED_DISTANCE 500

void GetRand(int num, int min, int max, vector<int>& rand_data);
void InitAllUnits(vector<int> units, map<int, BaseUnit>& all_units, int map_size_row, int map_size_col);
void PrintAllUnit(map<int, BaseUnit> all_units, int time_stamp);
void PrintUnitRelated(map<int, BaseUnit> all_units);
void InitAllRegions(map<int, MapRegion>& all_regions, int map_size_row, int map_size_col, int radius);
void PrintRegions(map<int, MapRegion> all_regions);
map<int, vector<int>> GetChangeTime(int rand_kind, int time, int num, map<int, BaseUnit> all_units);
void PrintChangeTime(map<int, vector<int>> change_times);
double CalculateDistance(double x1, double y1, double z1, double x2, double y2, double z2);
void GetMapRegion(BaseUnit& unit, map<int, MapRegion>& map_regions);
void RefreshMapRegionUnits(map<int, BaseUnit>& all_units, map<int, MapRegion>& all_regions);
void PrintMapRegionUnits(map<int, MapRegion> all_regions);
void ChangeSpeed(BaseUnit& unit, double time_slice, double new_speed, double directionX, double directionY, double directionZ, double map_sizeX, double map_sizeY, double map_sizeZ);
void InitRelatedUnits(map<int, BaseUnit>& all_units);
void ClearRelatedObjects(map<int, BaseUnit> units, map<int, BaseUnit> all_units);
void InitRelatedRegions(map<int, MapRegion>& all_regions, int radius);
void RefreshUnitsRelated(map<int, BaseUnit> units, map<int, BaseUnit> all_units, map<int, MapRegion> all_regions);
#endif // UTILS_H
