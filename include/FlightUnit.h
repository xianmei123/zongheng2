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

    void move(double timeSlice);            // 移动对象的方法
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
                       
    void setCamp(int camp); 
    void setStatus(bool status);
    void setPosition(double x, double y, double z);   
    void setSpeed(double speed);                       
    void setDirection(double x, double y, double z);   
    void setSize(double size);         
    void setCommunicationDistance(double distance);
    void addRelatedObjectID(int objectID);
    int generateUID();         

private:
    int ID_;                             // 飞行单位ID
    int camp_;                           // 所属阵营
    bool status_;                        // 对象的状态（活动/死亡）
    double positionX_;                   // 位置坐标的X分量
    double positionY_;                   // 位置坐标的Y分量
    double positionZ_;                   // 位置坐标的Z分量
    double speed_;                       // 速度
    double directionX_;                  // 运动方向的X分量
    double directionY_;                  // 运动方向的Y分量
    double size_;                        // 单位大小
    double communication_distance_;      // 通讯距离
    std::vector<int> related_objectIDs_;  // 与其关联的对象ID列表
};

#endif // FLIGHTUNIT_H
