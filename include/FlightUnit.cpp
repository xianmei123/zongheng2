#include "FlightUnit.h"
#include "UniqueIDGenerator.h"
#include <cstdlib>
#include <ctime>
#include <vector>

// 构造函数
FlightUnit::FlightUnit(int id) 
    : ID(id) {
    // 初始化其他属性
    positionX = 0.0;
    positionY = 0.0;
    positionZ = 0.0;
    speed = 0.0;
    directionX = 0.0;
    directionY = 0.0;
    directionZ = 0.0;
    size = 0.0;
    camp = 0; // 默认阵营为0（中立）
    communication_distance = 0.0; // 初始化通讯距离为0
    status = true; // 默认状态为活动状态

    relatedObjectIDs = std::vector<int>();
}

// 析构函数
FlightUnit::~FlightUnit() {
    // 析构函数代码，如果有的话
}

// 获取属性的方法实现
int FlightUnit::getID() const {
    return id;
}

int FlightUnit::getCamp() const {
    return camp;
}

bool FlightUnit::getStatus() const {
    return status;
}

double FlightUnit::getPositionX() const {
    return positionX;
}

double FlightUnit::getPositionY() const {
    return positionY;
}

double FlightUnit::getPositionZ() const {
    return positionZ;
}

double FlightUnit::getSpeed() const {
    return speed;
}

double FlightUnit::getDirectionX() const {
    return directionX;
}

double FlightUnit::getDirectionY() const {
    return directionY;
}

double FlightUnit::getDirectionZ() const {
    return directionZ;
}

double FlightUnit::getSize() const {
    return size;
}

double FlightUnit::getCommunicationDistance() const {
    return communication_distance;
}

const std::vector<int>& FlightUnit::getRelatedObjectIDs() const {
    return relatedObjectIDs;
}

// 设置属性的方法实现
void FlightUnit::setStatus(bool status) {
    this->status = status;
}

void FlightUnit::setPosition(double x, double y, double z) {
    positionX = x;
    positionY = y;
    positionZ = z;
}

void FlightUnit::setSpeed(double speed) {
    this->speed = speed;
}

void FlightUnit::setDirection(double x, double y, double z) {
    directionX = x;
    directionY = y;
    directionZ = z;
}

void FlightUnit::setSize(double size) {
    this->size = size;
}

void FlightUnit::setCamp(int camp) {
    this->camp = camp;
}

void FlightUnit::addRelatedObjectID(int objectID) {
    relatedObjectIDs.push_back(objectID);
}

// 生成唯一ID的静态方法实现 (简单的自增计数器)
int FlightUnit::generateUID() {
    static int uidCounter = 1;
    return uidCounter++;
}

// 移动对象的方法实现
void FlightUnit::move(double timeSlice) {
    // 根据速度和运动方向更新位置
    double direction = sqrt(this->directionX*this->directionX + this->directionY*this->directionY + this->directionZ*this->directionZ);

    this->positionX += this->speed * timeSlice * directionX / direction;
    this->positionY += this->speed * timeSlice * directionX / direction;
    this->positionZ += this->speed * timeSlice * directionX / direction;
}

// 改变对象速度的方法实现
void FlightUnit::changeSpeed(double timeSlice, double speed, double directionX, double directionY, double directionZ, double mapSizeX, double mapSizeY, double mapSizeZ) {
    // 在实际应用中，根据需求实现改变速度的逻辑
    // 这里只是一个示例
    this->speed = speed;

    double direction = sqrt(directionX*directionX + directionY*directionY + directionZ*directionZ);

    double nextPosX = this->positionX + timeSlice * this->speed * directionX / direction;
    double nextPosY = this->positionY + timeSlice * this->speed * directionY / direction;
    double nextPosZ = this->positionZ + timeSlice * this->speed * directionZ / direction;

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

void FlightUnit::setCommunicationDistance(double distance) {
    communication_distance = distance;
}
