// NonMovableBase.h
#ifndef NONMOVABLEBASE_H
#define NONMOVABLEBASE_H

#include <vector> // 包含vector头文件

class NonMovableBase {
public:
    NonMovableBase(); // 默认构造函数
    void move();            // 移动对象的方法
    void change_speed();    // 改变对象速度的方法

    // 属性的getter和setter方法
    int getID() const;
    double getPositionX() const;
    double getPositionY() const;
    double getPositionZ() const;
    double getVelocity() const;
    bool getStatus() const;
    double getCommunicationDistance() const;
    int getCamp() const;
    const std::vector<int>& getRelatedObjectIDs() const;

    void setPosition(double x, double y, double z);
    void setVelocity(double velocity);
    void setStatus(bool status);
    void setCommunicationDistance(double distance);
    void setCamp(int camp);
    void addRelatedObjectID(int objectID);

private:
    // 生成唯一的UID方法（可以根据需求自行实现）
    int generateUniqueID();

    // 属性
    int id;                   // 对象的唯一标识符
    double positionX;        // 对象的当前X坐标
    double positionY;        // 对象的当前Y坐标
    double positionZ;        // 对象的当前Z坐标
    double velocity;         // 对象的当前速度
    bool status;             // 对象的状态（活动/死亡等）
    double communication_distance; // 通讯距离
    int camp;             // 阵营归属
    std::vector<int> related_object_ids; // 与其有关联对象的ID列表
};

#endif
