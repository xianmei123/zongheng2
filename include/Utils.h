#ifndef UTILS_H
#define UTILS_H

#include "BaseUnit.h"
#include "MapRegion.h"
#include <vector>
#include <iostream>
#include <cstdlib>

//设置两个实体距离小于多少时，二者可以建立联系
#define CONNECTED_DISTANCE 500.00


// 计算两个点之间的欧几里德距离
double calculateDistance(double x1, double y1, double z1, double x2, double y2, double z2);

// 找到距离Object最近的MapRegion对象并返回
const MapRegion& getMapBlockIndex(const BaseUnit& object, const std::vector<MapRegion>& mapVector);

#endif // UTILS_H
