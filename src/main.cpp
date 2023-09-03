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
  // PrintAllUnit(all_units, 0);
  // PrintUnitRelated(all_units);
  saveAllUnitInfo(all_units, 0);   // 保存初始化的状态
  RefreshMapRegionUnits(all_units, all_regions);
  // PrintMapRegionUnits(all_regions);

  // 得到速度变化时刻表
  int rand_kind = 1;
  int time_length = 3000;
  int num = 100;
  int time_slice = 1;
  // for (auto unit : all_units) {
  //   unit.second.move(time_slice);
  // }
  for (unsigned i = 0; i < all_units.size(); ++i) {
    if (all_units.find(i)->second.getPriority() != 4) {
      all_units.find(i)->second.move(time_slice);
    }
  }
  //all_units.find(0)->second.move(time_slice);
  map<int, vector<int>> change_times = GetChangeTime(rand_kind, time_length, num, all_units);
  // PrintChangeTime(change_times);
  for (int i = 1; i < time_length; ++i) {
    // PrintAllUnit(all_units, i);
      saveAllUnitInfo(all_units, i);   // 保存每一时刻的状态
    if (change_times.find(i) != change_times.end()) {
      printf("%d\n", change_times[i][0]);
      auto unit = all_units.find(change_times[i][0])->second;
      printf("niuma\n");
      printf("%d %d %d %d \n", change_times[i][1], change_times[i][2], change_times[i][3], change_times[i][4]);
      printf("%d\n", all_units.find(change_times[i][0])->second.getPriority());
      if (all_units.find(change_times[i][0])->second.getPriority() != 4) {
        printf("<<<<");
        ChangeSpeed(all_units.find(change_times[i][0])->second, time_slice, change_times[i][1], change_times[1][2],
                    change_times[i][3], change_times[i][4], map_size_row, map_size_col, 0);
      }
    }
  }
  // for (int i = 1; i < time_length; ++i) {
  //   printf("what??");
  //   // PrintAllUnit(all_units, i);
  //   if (change_times.find(i) != change_times.end() && all_units.find(change_times[i][0])->second.getPriority() != 4) {
  //     printf(">>>>");
  //     all_units.find(change_times[i][0])->second.changeSpeed(time_slice, change_times[i][i], change_times[i][2], change_times[i][3], change_times[i][4],
  //                                                           map_size_row, map_size_col, 0);
  //   }
  //   for (unsigned j = 0; j < all_units.size(); ++j) {
  //     if (all_units.find(j)->second.getPriority() != 4) {
  //       all_units.find(j)->second.move(time_slice);
  //     }
  //   }
  //   printf(">>>>>..");
  //   RefreshMapRegionUnits(all_units, all_regions);
  //   map<int, BaseUnit> cur_units;
  //   printf(">>>>>......");
  //   if (i % 5 == 0) {
  //     cur_units = all_units;
  //   } else if (i % 2 == 0) {
  //     for (auto unit : all_units) {
  //       if (unit.second.getPriority() == 1 || unit.second.getPriority() == 2) {
  //         cur_units.find(unit.first)->second = unit.second;
  //       }
  //     }
  //   } else {
  //     map<int, BaseUnit> units;
  //     for (auto unit : all_units) {
  //       if (unit.second.getPriority() == 1) {
  //         cur_units.find(unit.first)->second = unit.second;
  //       }
  //     }
  //   }
  //   RefreshUnitsRelated(cur_units, all_units, all_regions);
  //   printf(">>>>>.,.,..");
  // }

  return 0;
}