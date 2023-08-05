#ifndef GROUNDUNIT_H
#define GROUNDUNIT_H

#include "BaseUnit.h"
#include <string>
#include <vector>

class GroundUnit: public BaseUnit {
public:
    // 构造函数
    GroundUnit(int id);

    GroundUnit(int id, double position_x, double position_y, double position_z, double speed);

    // 析构函数 对象删除前，保存最终的状态
    virtual ~GroundUnit();
    // 属性的getter和setter方法

private:
};

#endif // GROUNDUNIT_H
