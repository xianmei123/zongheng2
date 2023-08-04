#include "NonMovableUnit.h"
#include <vector>

// 构造函数
NonMovableUnit::NonMovableUnit(int id)
    : ID(id) {
        positionX = 0.0, 
        positionY = 0.0, 
        positionZ = 0.0, 
        status = true, 
        communication_distance = 0.0, 
        camp = 0,
        related_object_ids = std::vector<int>();
    }

// 析构函数
NonMovableUnit::~NonMovableUnit() {}

// 属性的getter方法实现
int NonMovableUnit::getID() const {
    return ID;
}

double NonMovableUnit::getPositionX() const {
    return positionX;
}

double NonMovableUnit::getPositionY() const {
    return positionY;
}

double NonMovableUnit::getPositionZ() const {
    return positionZ;
}

bool NonMovableUnit::getStatus() const {
    return status;
}

double NonMovableUnit::getCommunicationDistance() const {
    return communication_distance;
}

int NonMovableUnit::getCamp() const {
    return camp;
}

const std::vector<int>& NonMovableUnit::getRelatedObjectIDs() const {
    return related_object_ids;
}

// 属性的setter方法实现
void NonMovableUnit::setPosition(double x, double y, double z) {
    positionX = x;
    positionY = y;
    positionZ = z;
}

void NonMovableUnit::setStatus(bool status) {
    this->status = status;
}

void NonMovableUnit::setCommunicationDistance(double distance) {
    communication_distance = distance;
}

void NonMovableUnit::setCamp(int camp) {
    this->camp = camp;
}

void NonMovableUnit::addRelatedObjectID(int objectID) {
    related_object_ids.push_back(objectID);
}
