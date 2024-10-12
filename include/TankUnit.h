#ifndef TANKUNIT_H
#define TANKUNIT_H

#include "GroundUnit.h"

class TankUnit: public GroundUnit {
public:
  // 构造函数
  using GroundUnit::GroundUnit;

  void Init(int region_start_x, int region_start_y, int region_size_x, int region_size_y, int map_size_z);
  void Update(int time_slice, map<int, shared_ptr<BaseUnit>> all_units,
              double *positions, double *target_positions, double *directions, double *distances, int *target_ids) override;

protected:
};

#endif // TANKUNIT_H
