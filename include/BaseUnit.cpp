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

double BaseUnit::getSpeed() const {
    return speed;
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

int BaseUnit::getPriority() const {
    return priority;
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

// 移动对象的方法实现
void BaseUnit::move(double timeSlice) {
    // 根据速度和运动方向更新位置
    double direction = sqrt(this->directionX*this->directionX + this->directionY*this->directionY + this->directionZ*this->directionZ);

    this->positionX += this->speed * timeSlice * directionX / direction;
    this->positionY += this->speed * timeSlice * directionY / direction;
    this->positionZ += this->speed * timeSlice * directionZ / direction;
}

// 改变对象速度的方法实现
void BaseUnit::changeSpeed(double timeSlice, double speed, double directionX, double directionY, double directionZ, double mapSizeX, double mapSizeY, double mapSizeZ) {
    // 在实际应用中，根据需求实现改变速度的逻辑
    // 这里只是一个示例
    this->speed = speed;
    // // 移动
    double direction = sqrt(this->directionX*this->directionX + this->directionY*this->directionY + this->directionZ*this->directionZ);

    double nextPosX = this->positionX + this->speed * timeSlice * directionX / direction;
    double nextPosY = this->positionY + this->speed * timeSlice * directionY / direction;
    double nextPosZ = this->positionZ + this->speed * timeSlice * directionZ / direction;
    // this.move(timeSlice)
    if(nextPosX > mapSizeX || nextPosX < 0){
        directionX = -directionX;
    }
    if(nextPosY > mapSizeY || nextPosY < 0){
        directionY = -directionY;
    }
    if(nextPosZ > mapSizeZ || nextPosZ < 0){
        directionZ = -directionZ;
    }
    this->directionX = directionX;
    this->directionY = directionY;
    this->directionZ = directionZ;


}