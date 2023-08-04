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

using namespace std;
int main() {
<<<<<<< Updated upstream
  //随机生成四个类的比例，根据比例生成类的对象，分别存在
  // 赶紧写quickly
=======
  // 所有类的总个数
  int unit_num = 1000;
  int unit_kind = 4;
  vector<int> units(unit_kind);
  map<int, FlightUnit> fast_flight_units;
  map<int, FlightUnit> slow_flight_units;
  map<int, GroundUnit> ground_units;
  map<int, NonMovableUnit> non_movable_units;
  // 随机生成四个类的比例，根据比例生成类的对象，分别存在
  srand(time(0));
  vector<int> rand_data = GetRand(unit_kind, 1, 10);
  for (int i = 0; i < unit_kind; ++i) {
    units.push_back(unit_num * rand_data[i] / rand_data.back());
  }
  //  地图格点的长和宽
  int map_size_row = 10000;
  int map_size_col = 10000;
  // 开始生成对象, 按照速度大小范围依次生成对象
  int cur_index = 0; // 已经添加的对象个数
  for (int index = 0; index < units[0]; ++index) {  // 速度50~199
    fast_flight_units[index] = FlightUnit(index, rand() % map_size_row, rand() % map_size_col, 0, 50 + rand() % 150);
  }
  cur_index += units[0];
  for (int index = 0; index < units[1]; ++index) {  // 速度10~49
    slow_flight_units[index] = FlightUnit(index + cur_index, rand() % map_size_row, rand() % map_size_col, 0, 10 + rand() % 40);
  }
  cur_index += units[1];
  for (int index = 0; index < units[2]; ++index) {  // 速度0~9
    ground_units[index] = GroundUnit(index + cur_index, rand() % map_size_row, rand() % map_size_col, 0, 0 + rand() % 10);
  }
  cur_index += units[2];
  for (int index = 0; index < units[3]; ++index) {
    non_movable_units[index] = NonMovableUnit(index + cur_index, rand() % map_size_row, rand() % map_size_col, 0);
  }

  // 
  return 0;
}

vector<int> GetNewSpeedAndDirection(int index) {

}

map<int, vector<int>> GetChangeTime(int rand_kind, int time, int num) {
>>>>>>> Stashed changes
  
  if (rand_kind == 1) { // 均匀分布
    vector<int> timestamps = GetRand(num, 0, time);
    sort(timestamps.begin(), timestamps.end());
    for (int i = 0; i < num; ++i) {
      switch (expression)
      {
      case /* constant-expression */:
        /* code */
        break;
      
      default:
        break;
      }
    }
  } else if (rand_kind == 2) {  // 正态分布

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