// NonMovableUnit.cpp
#include "NonMovableUnit.h"

NonMovableUnit::NonMovableUnit() {
    // 在构造函数中生成唯一的ID
    id = generateUniqueID();
    positionX = 0.0;
    positionY = 0.0;
    positionZ = 0.0;
    velocity = 0.0;
    status = true; // 假设true表示活动，false表示死亡
    communication_distance = 0.0; // 初始化通讯距离为0
    camp = 0; // 初始化阵营归属为0（可以根据需求定义阵营标识）
}

void NonMovableUnit::move() {
    // 实现move方法，根据需要进行移动逻辑
    // 对于不可移动的对象，该方法可以为空，或者不做任何操作
}

void NonMovableUnit::change_speed() {
    // 实现change_speed方法，根据需要修改速度逻辑
    // 为简单起见，我们将速度设置为固定值10单位
    velocity = 10.0;
}

// 生成唯一的UID方法的实现（根据需求自行实现）
int NonMovableUnit::generateUniqueID() {
    // 实现生成唯一ID的逻辑，例如使用时间戳、随机数生成器等方式
    // 这里简化为返回固定的ID 1
    return 1;
}

// 属性的getter和setter方法的实现
// ... 省略其他属性的getter和setter方法实现 ...

double NonMovableUnit::getCommunicationDistance() const {
    return communication_distance;
}

int NonMovableUnit::getCamp() const {
    return camp;
}

const std::vector<int>& NonMovableUnit::getRelatedObjectIDs() const {
    return related_object_ids;
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
