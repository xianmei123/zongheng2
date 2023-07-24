#include "MapRegion.h"
#include <vector>

// 构造函数
MapRegion::MapRegion(int id) 
    : ID(id) {
    // 初始化其它属性
    centerX = 0.0
    centerY = 0.0
    centerZ = 0.0
    radius = 1.0

    relatedMapUnitIDs = std::vector<int>()
    includeObjectIDs = std::vector<int>()
}

MapRegion::MapRegion(int id, double centerX, double centerY, double centerZ, double radius)
    : ID(id), centerX(centerX), centerY(centerY), centerZ(centerZ), radius(radius) {}

// 获取属性的方法
int MapRegion::getID() const {
    return ID;
}

double MapRegion::getCenterX() const {
    return centerX;
}

double MapRegion::getCenterY() const {
    return centerY;
}

double MapRegion::getCenterZ() const {
    return centerZ;
}

double MapRegion::getRadius() const {
    return radius;
}

const std::vector<int>& MapRegion::getRelatedMapUnitIDs() const {
    return relatedMapUnitIDs;
}

const std::vector<int>& MapRegion::getIncludeObjectIDs() const {
    return includeObjectIDs;
}

// 设置属性的方法
void MapRegion::addRelatedMapUnitID(int mapUnitID) {
    relatedMapUnitIDs.push_back(mapUnitID);
}

void MapRegion::addIncludeObjectID(int objectID) {
    includeObjectIDs.push_back(objectID);
}
