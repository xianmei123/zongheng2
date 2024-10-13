#ifndef GROUNDUNIT_H
#define GROUNDUNIT_H

#include "BaseUnit.h"

class GroundUnit: public BaseUnit {
public:
  // 构造函数
  using BaseUnit::BaseUnit;

protected:
  double init_positionX_;
  double init_positionY_;
  double init_positionZ_;
};

#endif // GROUNDUNIT_H
