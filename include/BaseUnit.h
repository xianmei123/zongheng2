#ifndef BASEUNIT_H
#define BASEUNIT_H

#include <string>
#include <vector>

class BaseUnit {
public:
    // 构造函数
    BaseUnit(int id);
    BaseUnit(int id, double speed, double directionX, double directionY, double directionZ, double positionX, double positionY, double getPositionZ);

    // 析构函数 对象删除前，保存最终的状态
    ~BaseUnit();

    void changeSpeed(double timeSlice, double speed, double directionX, double directionY, double directionZ, double mapSizeX, double mapSizeY, double mapSizeZ);    // 改变对象速度的方法
    void move(double timeSlice);            // 移动对象的方法

    // 属性的getter和setter方法
    int getID() const;                             
    int getCamp() const;                           
    bool getStatus() const;
    double getSpeed() const;

    double getDirectionX() const;
    double getDirectionY() const;
    double getDirectionZ() const;
    double getPositionX() const;                   
    double getPositionY() const;                   
    double getPositionZ() const;                                 
    double getSize() const;        
    double getCommunicationDistance() const;
    int getPriority() const;

    void setStatus(bool status);
    void setPosition(double x, double y, double z);   
    void setSize(double size);                        
    void setCamp(int camp);         
    void setCommunicationDistance(double distance);
    void setDirection(double x, double y, double z);
    void setSpeed(double speed);
    void setPriority(int priority);

    const std::vector<int>& getRelatedObjectIDs() const; 
    void addRelatedObjectID(int objectID);    
    void clearRelatedObjectID();  
    void deleteRelatedObjectID(int id); 
private:
    int ID;                             // 单位ID
    int camp;                           // 所属阵营
    bool status;                        // 对象的状态（活动/死亡）
    double positionX;                   // 位置坐标的X分量
    double positionY;                   // 位置坐标的Y分量
    double positionZ;                   // 位置坐标的Z分量
    double size;                        // 单位大小
    double communication_distance;      // 通讯距离
    double directionX;                  // 运动方向的X分量
    double directionY;                  // 运动方向的Y分量
    double directionZ;                  // 运动方向的Z分量
    double speed;                       // 速度
    int priority;                       // 优先级
    std::vector<int> related_objectIDs_;  // 与其关联的对象ID列表

};

#endif // BASEUNIT_H
