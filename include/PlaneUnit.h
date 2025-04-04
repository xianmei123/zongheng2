#ifndef PLANEUNIT_H
#define PLANEUNIT_H

#include "FlightUnit.h"

class PlaneUnit: public FlightUnit {
public:
  // 构造函数
  using FlightUnit::FlightUnit;

  // 属性的getter和setter
  double getHitProbability() const;
  void setHitProbability(double hitProbability);

  void Init(int region_start_x, int region_start_y, int region_size_x, int region_size_y, int map_size_z);
  void Update(int time_slice, map<int, shared_ptr<BaseUnit>> all_units,
              double *positions, double *target_positions, double *directions, double *distances, int *target_ids, int* status) override;

protected:
  double hit_probability_;             // 武器命中概率
};

#endif // PLANEUNIT_H
