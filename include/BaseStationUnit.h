#ifndef BASESTATIONUNIT_H
#define BASESTATIONUNIT_H

#include "NonMovableUnit.h"

class BaseStationUnit: public NonMovableUnit {
public:
  // 构造函数
  using NonMovableUnit::NonMovableUnit;

  void Init(int region_start_x, int region_start_y, int region_size_x, int region_size_y, int map_size_z);
  void Update(int time_slice, map<int, std::shared_ptr<BaseUnit>> all_units, 
              double *positions, double *target_positions, double *directions, double *distances, int *target_ids, int *status) override;

private:
};

#endif // BASESTATIONUNIT_H
