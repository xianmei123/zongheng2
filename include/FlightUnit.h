#ifndef FLIGHTUNIT_H
#define FLIGHTUNIT_H

#include <string>
#include <vector>

class FlightUnit {
public:
    // 构造函数
    FlightUnit();

    // 析构函数
    ~FlightUnit();

    void move();            // 移动对象的方法
    void change_speed();    // 改变对象速度的方法

    // 属性的getter和setter方法
    int getID() const;                             
    int getCamp() const;                           
    bool getStatus() const;

    double getPositionX() const;                   
    double getPositionY() const;                   
    double getPositionZ() const;                   
    double getSpeed() const;                       
    double getDirectionX() const;                  
    double getDirectionY() const;                  
    double getDirectionZ() const;                  
    double getSize() const;                            
    const std::vector<int>& getRelatedObjectIDs() const; 

    void setStatus(bool status);
    void setPosition(double x, double y, double z);   
    void setSpeed(double speed);                       
    void setDirection(double x, double y, double z);   
    void setSize(double size);                        
    void setCamp(int camp);            
    void addRelatedObjectID(int objectID);           

private:
    int id;                             // 飞行单位ID
    int camp;                           // 所属阵营
    bool status;                        // 对象的状态（活动/死亡等）
    double positionX;                   // 位置坐标的X分量
    double positionY;                   // 位置坐标的Y分量
    double positionZ;                   // 位置坐标的Z分量
    double speed;                       // 速度
    double directionX;                  // 运动方向的X分量
    double directionY;                  // 运动方向的Y分量
    double directionZ;                  // 运动方向的Z分量
    double size;                        // 单位大小
    std::vector<int> relatedObjectIDs;  // 与其关联的对象ID列表

    // 生成唯一的UID方法（可以根据需求自行实现）
    int generateUID();
};

#endif // FLIGHTUNIT_H
