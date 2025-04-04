#ifndef MISSLEUNIT_H
#define MISSLEUNIT_H

#include "FlightUnit.h"

class MissleUnit: public FlightUnit {
public:
  // 构造函数
  using FlightUnit::FlightUnit;

  void Init(int region_start_x, int region_start_y, int region_size_x, int region_size_y, int map_size_z);
  void Update(int time_slice, map<int, shared_ptr<BaseUnit>> all_units,
              double *positions, double *target_positions, double *directions, double *distances, int *target_ids, int *status) override;

protected:
};

#endif // MISSLEUNIT_H
