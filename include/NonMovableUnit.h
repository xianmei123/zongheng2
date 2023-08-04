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
    int getID() const;
    double getPositionX() const;
    double getPositionY() const;
    double getPositionZ() const;
    bool getStatus() const;
    double getCommunicationDistance() const;
    int getCamp() const;
    const std::vector<int>& getRelatedObjectIDs() const;

    void setPosition(double x, double y, double z);
    void setStatus(bool status);
    void setCommunicationDistance(double distance);
    void setCamp(int camp);
    void addRelatedObjectID(int objectID);

private:
    // 属性
    int ID;                   // 对象的唯一标识符
    double positionX;        // 对象的当前X坐标
    double positionY;        // 对象的当前Y坐标
    double positionZ;        // 对象的当前Z坐标
    bool status;             // 对象的状态（活动/死亡）
    double communication_distance; // 通讯距离
    int camp;             // 阵营归属
    std::vector<int> related_object_ids; // 与其有关联对象的ID列表
};

#endif
