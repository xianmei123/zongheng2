#ifndef MAPREGION_H
#define MAPREGION_H

#include <vector>

class MapRegion {
public:
    // 构造函数
    MapRegion(int id);
    MapRegion(int id, double centerX, double centerY, double centerZ, double radius);

    // 获取属性的方法
    int getID() const;                      // 获取区域ID
    double getCenterX() const;              // 获取区域中心X坐标
    double getCenterY() const;              // 获取区域中心Y坐标
    double getCenterZ() const;              // 获取区域中心Z坐标
    double getRadius() const;               // 获取区域半径
    const std::vector<int>& getRelatedMapUnitIDs() const;    // 获取相关地图单元ID数组
    const std::vector<int>& getIncludeObjectIDs() const;     // 获取包含对象ID数组

    // 设置属性的方法
    void addRelatedMapUnitID(int mapUnitID);    // 添加相关地图单元ID
    void addSubRelatedMapUnitIDs() // 添加半相关地图单元ID数组
    void addIncludeObjectID(int objectID);      // 添加包含对象ID
    void clearIncludeObjectID();

private:
    int ID;                               // 区域ID
    double centerX;                       // 区域中心X坐标
    double centerY;                       // 区域中心Y坐标
    double centerZ;                       // 区域中心Z坐标
    double radius;                        // 区域半径
    std::vector<int> relatedMapUnitIDs;   // 相关地图单元ID数组
    std::vector<int> subRelatedMapUnitIDs; // 半相关地图单元ID数组
    std::vector<int> includeObjectIDs;    // 包含对象ID数组
};

#endif // MAPREGION_H
