#ifndef GROUNDUNIT_H
#define GROUNDUNIT_H

#include <string>
#include <vector>

class GroundUnit {
public:
    // 构造函数
    GroundUnit(int id);

    // 析构函数 对象删除前，保存最终的状态
    ~GroundUnit();

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
    double getCommunicationDistance() const;
    const std::vector<int>& getRelatedObjectIDs() const; 

    void setStatus(bool status);
    void setPosition(double x, double y, double z);   
    void setSpeed(double speed);                       
    void setDirection(double x, double y, double z);   
    void setSize(double size);                        
    void setCamp(int camp);         
    void setCommunicationDistance(double distance);
    void addRelatedObjectID(int objectID);           

private:
    int ID;                             // 飞行单位ID
    int camp;                           // 所属阵营
    bool status;                        // 对象的状态（活动/死亡）
    double positionX;                   // 位置坐标的X分量
    double positionY;                   // 位置坐标的Y分量
    double positionZ;                   // 位置坐标的Z分量
    double speed;                       // 速度
    double directionX;                  // 运动方向的X分量
    double directionY;                  // 运动方向的Y分量
    double directionZ;                  // 运动方向的Z分量
    double size;                        // 单位大小
    double communication_distance;      // 通讯距离

    std::vector<int> relatedObjectIDs;  // 与其关联的对象ID列表

    // 生成唯一的UID方法（可以根据需求自行实现）
    int generateUID();
};

#endif // GROUNDUNIT_H
