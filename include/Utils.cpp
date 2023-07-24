#include "Utils.h"
#include <cmath>

// 计算两个点之间的欧几里德距离
double calculateDistance(double x1, double y1, double z1, double x2, double y2, double z2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    double dz = z2 - z1;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

// 找到距离Object最近的MapRegion对象并返回
const MapRegion& getMapBlockIndex(const BaseUnit& object, const std::vector<MapRegion>& mapVector) {
    const double objectX = object.getPositionX();
    const double objectY = object.getPositionY();
    const double objectZ = object.getPositionZ();
    double minDistance = std::numeric_limits<double>::max();
    const MapRegion* closestMapRegion = nullptr;

    for (const auto& mapRegion : mapVector) {
        double regionCenterX = mapRegion.getCenterX();
        double regionCenterY = mapRegion.getCenterY();
        double regionCenterZ = mapRegion.getCenterZ();

        double distance = calculateDistance(objectX, objectY, objectZ, regionCenterX, regionCenterY, regionCenterZ);

        if (distance < minDistance) {
            minDistance = distance;
            closestMapRegion = &mapRegion;
        }
    }

    if (closestMapRegion == nullptr) {
        // Handle the case when the mapVector is empty or invalid.
        // You may throw an exception or return a default MapRegion object.
    }

    return *closestMapRegion;
}


void updateSpeed(const BaseUnit& object, double speed, double directionX, double directionY, double directionZ, double mapSizeX, double mapSizeY, double mapSizeZ){
    const double objectX = object.getPositionX();
    const double objectY = object.getPositionY();
    const double objectZ = object.getPositionZ();

    if(objectX + object.)
    
}