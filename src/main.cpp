#include <iostream>
#include <ctime>
#include <cstdlib>
#include <map>
#include <vector>
#include <time.h>
#include <algorithm>
#include "../include/FlightUnit.h"
#include "../include/GroundUnit.h"
#include "../include/NonMovableUnit.h"
#include "../include/BaseUnit.h"
#include "../include/Utils.h"

using namespace std;
int main() {
  // 所有类的总个数
  int unit_num = 1000;
  int unit_kind = 4;
  vector<int> units(unit_kind);
  map<int, BaseUnit> all_units;
  map<int, MapRegion> all_regions;
  // 随机生成四个类的比例，根据比例生成类的对象，分别存在
  srand(time(0));
  vector<int> rand_data = GetRand(unit_kind, 1, 10);
  for (int i = 0; i < unit_kind; ++i) {
    units.push_back(unit_num * rand_data[i] / rand_data.back());
  }
  //  地图格点的长和宽
  int map_size_row = 10000;
  int map_size_col = 10000;
  int region_size = 100;
  int radius = 100;
  // 开始生成对象, 按照速度大小范围依次生成对象
  int cur_id = 0; // 已经添加的对象个数
  for (int id = 0; id < units[0]; ++id) {  // 速度50~199
    all_units[id] = FlightUnit(id, rand() % map_size_row, rand() % map_size_col, 0, 50 + rand() % 150);
    all_units[id].setPriority(1);
  }
  cur_id += units[0];
  for (int id = 0; id < units[1]; ++id) {  // 速度10~49
    all_units[id + cur_id] = FlightUnit(id + cur_id, rand() % map_size_row, rand() % map_size_col, 0, 10 + rand() % 40);
    all_units[id].setPriority(2);
  }
  cur_id += units[1];
  for (int id = 0; id < units[2]; ++id) {  // 速度0~9
    all_units[id + cur_id] = GroundUnit(id + cur_id, rand() % map_size_row, rand() % map_size_col, 0, 0 + rand() % 10);
    all_units[id].setPriority(3);
  }
  cur_id += units[2];
  for (int id = 0; id < units[3]; ++id) {
    all_units[id + cur_id] = NonMovableUnit(id + cur_id, rand() % map_size_row, rand() % map_size_col, 0);
  }
  int region_num = 0;
  for (int i = 0; i < map_size_row / region_size; ++i) {
    for (int j = 0; j < map_size_col / region_size; ++j) {
      all_regions[region_num] = MapRegion(region_num, i * region_size + radius / 2, j * region_size + radius / 2, 0, radius); 
      region_num ++;
    }
  }
  for (int i = 0; i < region_num; ++i) {
    for (int j = 0; j < region_num; ++j) {
        double distance = calculateDistance(all_regions[i].getCenterX(), all_regions[i].getCenterY(), all_regions[i].getCenterZ(), 
                                            all_regions[j].getCenterX(), all_regions[j].getCenterY(), all_regions[j].getCenterZ());
        if (distance < CONNECTED_DISTANCE) {
          all_regions[i].addRelatedMapUnitID(j);
        } else if (distance < CONNECTED_DISTANCE + radius) {
          all_regions[i].addRelatedMapUnitID(j);
        }
    }
  }
  refreshMapRegionUnits(all_units, all_regions);
  InitRelatedObjectIDs(all_units);
  // 得到速度变化时刻表
  int rand_kind = 1;
  int time = 3000;
  int num = 100;
  int timeSlice = 1;
  map<int, vector<int>> changeTime = GetChangeTime(rand_kind, time, num, unit_num, all_units);
  for (int i = 0; i < time; ++i) {
    if (changeTime.find(i) != changeTime.end()) {
      changeSpeed(timeSlice, changeTime[i][0], changeTime[i][1], changeTime[i][2], changeTime[i][3]);
    }
    move(timeSlice);
    refreshMapRegionUnits(all_units, all_regions);
    if (i % 5 == 0) {
      map<int, BaseUnit> cur_units = all_units;
    } else if (i % 2 == 0) {
      map<int, BaseUnit> cur_units;
      for (int j = 0; j < all_units.size(); ++j) {
        if (all_units[j].getPriority() == 1 || all_units[j].getPriority() == 2) {
          cur_units[j] = all_units[j];
        }
      }
    } else {
      map<int, BaseUnit> cur_units;
      for (int j = 0; j < all_units.size(); ++j) {
        if (all_units[j].getPriority() == 1) {
          cur_units[j] = all_units[j];
        }
      }
    }
    refresh_entity_connection(all_units, cur_units, all_regions);
  }
  // 
  return 0;
}


vector<int> GetNewSpeedAndDirection(int id, map<int, BaseUnit> all_units) {
  vector<int> newSpeedAndDirection;
  if (all_units[id].getPriority() == 1) {
    newSpeedAndDirection.push_back(50 + rand() % 150);
  } else if (all_units[id].getPriority() == 2) {
    newSpeedAndDirection.push_back(10 + rand() % 40);
  } else if (all_units[id].getPriority() == 3) {
    newSpeedAndDirection.push_back(rand() % 10);
  } else {
    printf("Error type to change speed!");
  }
  newSpeedAndDirection.insert(newSpeedAndDirection.end(), {rand() % 10, rand() % 10, rand() % 10});

  return newSpeedAndDirection;
}

// 该map返回一个map，key是需要进行改变的时刻，该key对应的vector中包含该时刻需要改变的对象id，以及该对象的新速度和方向
map<int, vector<int>> GetChangeTime(int rand_kind, int time, int num, int unit_num, map<int, BaseUnit> all_units) {
  
  if (rand_kind == 1) { // 均匀分布
    vector<int> timestamps = GetRand(num, 0, time);
    sort(timestamps.begin(), timestamps.end());
    vector<int> idNewSpeedAnddirection;
    for (int i = 0; i < num; ++i) {
      int changeId = rand() % (unit_num + 1);
      idNewSpeedAnddirection.push_back(changeId);
      vector<int> newSpeedAndDirection = GetNewSpeedAndDirection(changeId, all_units);
      idNewSpeedAnddirection.insert(idNewSpeedAnddirection.end(), newSpeedAndDirection.begin(), newSpeedAndDirection.end());
      map[timestamps[i]] = idNewSpeedAnddirection;
    }
  }
}

vector<int> GetRand(int num, int min, int max) {
  // 生成num个随机数，最后一个元素是所有随机数的和
  vector<int> rand_data(num + 1);
  int sum = 0;
  for (int i = 0; i < num; ++i) {
    // 生成1~10的随机数
    int cur_data = min + rand() % max;
    rand_data.push_back(cur_data);
    sum += cur_data;
  }
  rand_data.push_back(sum);

  return rand_data;
}