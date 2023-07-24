#include "GroundUnit.h"
#include <cstdlib>
#include <ctime>

// 构造函数
GroundUnit::GroundUnit(int id) 
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
}

// 析构函数
GroundUnit::~GroundUnit() {
    // 析构函数代码，如果有的话
}

// 获取属性的方法实现
int GroundUnit::getID() const {
    return id;
}

int GroundUnit::getCamp() const {
    return camp;
}

bool GroundUnit::getStatus() const {
    return status;
}

double GroundUnit::getPositionX() const {
    return positionX;
}

double GroundUnit::getPositionY() const {
    return positionY;
}

double GroundUnit::getPositionZ() const {
    return positionZ;
}

double GroundUnit::getSpeed() const {
    return speed;
}

double GroundUnit::getDirectionX() const {
    return directionX;
}

double GroundUnit::getDirectionY() const {
    return directionY;
}

double GroundUnit::getDirectionZ() const {
    return directionZ;
}

double GroundUnit::getSize() const {
    return size;
}

double GroundUnit::getCommunicationDistance() const {
    return communication_distance;
}

const std::vector<int>& GroundUnit::getRelatedObjectIDs() const {
    return relatedObjectIDs;
}

// 设置属性的方法实现
void GroundUnit::setStatus(bool status) {
    this->status = status;
}

void GroundUnit::setPosition(double x, double y, double z) {
    positionX = x;
    positionY = y;
    positionZ = z;
}

void GroundUnit::setSpeed(double speed) {
    this->speed = speed;
}

void GroundUnit::setDirection(double x, double y, double z) {
    directionX = x;
    directionY = y;
    directionZ = z;
}

void GroundUnit::setSize(double size) {
    this->size = size;
}

void GroundUnit::setCamp(int camp) {
    this->camp = camp;
}

void GroundUnit::addRelatedObjectID(int objectID) {
    relatedObjectIDs.push_back(objectID);
}

// 生成唯一ID的静态方法实现 (简单的自增计数器)
int GroundUnit::generateUID() {
    static int uidCounter = 1;
    return uidCounter++;
}

// 移动对象的方法实现
void GroundUnit::move() {
    // 根据速度和运动方向更新位置
    positionX += speed * directionX;
    positionY += speed * directionY;
    positionZ += speed * directionZ;
}

// 改变对象速度的方法实现
void GroundUnit::change_speed() {
    // 在实际应用中，根据需求实现改变速度的逻辑
    // 这里只是一个示例
    speed += 1.0;
}

void GroundUnit::setCommunicationDistance(double distance) {
    communication_distance = distance;
}
