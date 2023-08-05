// NonMovableUnit.h
#ifndef NONMOVABLEUnit_H
#define NONMOVABLEUnit_H

#include <vector> // 包含vector头文件

class NonMovableUnit: public BaseUnit {
public:
    // 构造函数
    NonMovableUnit(int id);
    NonMovableUnit(int id, double position_x, double position_y, double position_z);

    // 析构函数
    virtual ~NonMovableUnit();

    // 属性的getter和setter方法

private:
    // 属性
};

#endif
