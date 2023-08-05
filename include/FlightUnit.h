#ifndef FLIGHTUNIT_H
#define FLIGHTUNIT_H

#include <string>
#include <vector>

class FlightUnit: public BaseUnit {
public:
    // 构造函数
    FlightUnit(int id);
    FlightUnit(int id, double position_x, double position_y, double position_z, double speed);
    // 析构函数 对象删除前，保存最终的状态
    virtual ~FlightUnit(); 

private:
};

#endif // FLIGHTUNIT_H
