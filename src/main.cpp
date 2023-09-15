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
  int unit_num = 300;
  int unit_kind = 4;
  vector<int> units;
  map<int, BaseUnit> all_units;
  map<int, MapRegion> all_regions;
  // 随机生成四个类的比例，根据比例生成类的对象，分别存在
  srand(time(0));
  vector<int> rand_data;
  GetRand(unit_kind, 1, 10, rand_data);
  for (int i = 0; i < unit_kind; ++i) {
    units.push_back((int)(unit_num * (float)rand_data[i] / rand_data.back()));
  }
  //  地图格点的长和宽
  int map_size_row = 10000;
  int map_size_col = 10000;
  int radius = 100;
  // 开始生成对象, 按照速度大小范围依次生成对象
  printf("Start initial...\n");
  InitAllRegions(all_regions, map_size_row, map_size_col, radius);
  InitRelatedRegions(all_regions, radius);
  // PrintRegions(all_regions);
  InitAllUnits(units, all_units, map_size_row, map_size_col);
  InitRelatedUnits(all_units);
  PrintAllUnit(all_units, 0);
  PrintUnitRelated(all_units);
  SaveAllUnitInfo(all_units, 0);   // 保存初始化的状态
  RefreshMapRegionUnits(all_units, all_regions);
  // PrintMapRegionUnits(all_regions);

  // 得到速度变化时刻表
  int rand_kind = 1;
  int time_length = 50;
  int num = 100;
  int time_slice = 1;
  // for (auto unit : all_units) {
  //   unit.second.move(time_slice);
  // }
  for (unsigned i = 0; i < all_units.size(); ++i) {
    if (all_units.find(i)->second.getPriority() != 4) {
      all_units.find(i)->second.move(time_slice, map_size_row, map_size_col, 0);
    }
  }
  //all_units.find(0)->second.move(time_slice);
  map<int, map<int, vector<int>>> change_times = GetChangeTime(rand_kind, time_length, num, all_units);
  PrintChangeTime(change_times);
  for (int i = 1; i < time_length; ++i) {
    // PrintAllUnit(all_units, i);
    RefreshMapRegionUnits(all_units, all_regions);
    vector<int> cur_ids;
    if (i % 5 == 0) {
      for (unsigned j = 0; j < all_units.size(); ++j) {
        if (all_units.find(j)->second.getPriority() != 4) {
          cur_ids.push_back(j);
        }
      }
    } else if (i % 2 == 0) {
      for (unsigned j = 0; j < all_units.size(); ++j) {
        if (all_units.find(j)->second.getPriority() == 1 || all_units.find(j)->second.getPriority() == 2) {
          cur_ids.push_back(j);
        }
      }
    } else {
      for (unsigned j = 0; j < all_units.size(); ++j) {
        if (all_units.find(j)->second.getPriority() == 1) {
          cur_ids.push_back(j);
        }
      }
    }
    RefreshUnitsRelated(cur_ids, all_units, all_regions);
    PrintUnitRelated(all_units);
    PrintAllUnit(all_units, i);
    SaveAllUnitInfo(all_units, i);   // 保存每一时刻的状态
    if (change_times.find(i) != change_times.end()) {
      auto units = change_times.find(i)->second;
      for (auto unit_id : units) {
        auto unit = all_units.find(unit_id.first)->second;
        if (unit.getPriority() != 4) {
          ChangeSpeed(unit, time_slice, unit_id.second[0], unit_id.second[1],
                      unit_id.second[2], unit_id.second[3], map_size_row, map_size_col, 0);
        }
      }
    }
    for (unsigned j = 0; j < all_units.size(); ++j) {
      if (all_units.find(j)->second.getPriority() != 4) {
        all_units.find(j)->second.move(time_slice, map_size_row, map_size_col, 0);
      }
    }
  }

  return 0;
}