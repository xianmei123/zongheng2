#include "BaseUnit.h"

// 构造函数
BaseUnit::BaseUnit(int id) : ID(id), 
                            camp(0), 
                            status(false), 
                            positionX(0.0), 
                            positionY(0.0), 
                            positionZ(0.0),
                            size(1.0), 
                            communication_distance(0.0), 
                            directionX(0), 
                            directionY(0), 
                            directionZ(0), 
                            speed(0) {}

BaseUnit::BaseUnit(int id, double speed, double directionX, double directionY, double directionZ, double positionX, double positionY, double getPositionZ) : ID(id), 
                    camp(0), 
                    status(false), 
                    positionX(positionX), 
                    positionY(positionY), 
                    positionZ(positionZ),
                    size(1.0), 
                    communication_distance(0.0), 
                    directionX(directionX), 
                    directionY(directionY), 
                    directionZ(directionZ), 
                    speed(speed) {}

// 析构函数 对象删除前，保存最终的状态
BaseUnit::~BaseUnit() {}

// 属性的getter方法
int BaseUnit::getID() const {
    return ID;
}

int BaseUnit::getCamp() const {
    return camp;
}

bool BaseUnit::getStatus() const {
    return status;
}

double BaseUnit::getPositionX() const {
    return positionX;
}

double BaseUnit::getPositionY() const {
    return positionY;
}

double BaseUnit::getPositionZ() const {
    return positionZ;
}

double BaseUnit::getSize() const {
    return size;
}

double BaseUnit::getCommunicationDistance() const {
    return communication_distance;
}

// 属性的setter方法
void BaseUnit::setStatus(bool status) {
    this->status = status;
}

void BaseUnit::setPosition(double x, double y, double z) {
    positionX = x;
    positionY = y;
    positionZ = z;
}

void BaseUnit::setSize(double size) {
    this->size = size;
}

void BaseUnit::setCamp(int camp) {
    this->camp = camp;
}

void BaseUnit::setCommunicationDistance(double distance) {
    communication_distance = distance;
}
