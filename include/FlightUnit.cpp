#include "FlightUnit.h"
#include <cstdlib>
#include <ctime>

// 构造函数
FlightUnit::FlightUnit() {
    // 生成唯一ID给飞行单位
    id = generateUID();
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
void FlightUnit::move() {
    // 根据速度和运动方向更新位置
    positionX += speed * directionX;
    positionY += speed * directionY;
    positionZ += speed * directionZ;
}

// 改变对象速度的方法实现
void FlightUnit::change_speed() {
    // 在实际应用中，根据需求实现改变速度的逻辑
    // 这里只是一个示例
    speed += 1.0;
}

void FlightUnit::setCommunicationDistance(double distance) {
    communication_distance = distance;
}
