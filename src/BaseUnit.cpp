#include "BaseUnit.h"

// 构造函数
BaseUnit::BaseUnit(int id) :id_(id), 
                            camp_(0), 
                            status_(true), 
                            positionX_(0), 
                            positionY_(0), 
                            positionZ_(0),
                            size_(1), 
                            priority_(1),
                            target_id_(0),
                            target_positionX_(0), 
                            target_positionY_(0), 
                            target_positionZ_(0),
                            directionX_(0), 
                            directionY_(0), 
                            directionZ_(0), 
                            speed_(0),
                            monitor_distance_(0),
                            communication_distance_(0) {}

BaseUnit::BaseUnit(int id, double positionX, double positionY, double positionZ):
                  id_(id), 
                  camp_(0), 
                  status_(true), 
                  positionX_(positionX), 
                  positionY_(positionY), 
                  positionZ_(positionZ),
                  size_(1), 
                  priority_(1),
                  target_id_(0),
                  target_positionX_(0), 
                  target_positionY_(0), 
                  target_positionZ_(0),
                  directionX_(0), 
                  directionY_(0), 
                  directionZ_(0), 
                  speed_(0),
                  monitor_distance_(0),
                  communication_distance_(0) {}

BaseUnit::BaseUnit(int id, double speed, double directionX, double directionY, double directionZ, double positionX, double positionY, double positionZ) : 
                  id_(id), 
                  camp_(0), 
                  status_(true), 
                  positionX_(positionX), 
                  positionY_(positionY), 
                  positionZ_(positionZ),
                  size_(1), 
                  priority_(1),
                  target_id_(0),
                  target_positionX_(0), 
                  target_positionY_(0), 
                  target_positionZ_(0),
                  directionX_(directionX), 
                  directionY_(directionY), 
                  directionZ_(directionZ), 
                  speed_(speed),
                  monitor_distance_(0),
                  communication_distance_(0) {}


// 移动对象的方法实现
void BaseUnit::move(double time_slice, double map_sizeX, double map_sizeY, double map_sizeZ) {
    // 根据速度和运动方向更新位置
  if (directionX_ == 0 && directionY_ == 0 && directionZ_ == 0) {
    return;
  }
  if (!status_) {
    return;
  }
  double direction = sqrt(pow(directionX_, 2) + pow(directionY_, 2) + pow(directionZ_, 2));
  double next_posX = positionX_ + speed_ * time_slice * directionX_ / direction;
  double next_posY = positionY_ + speed_ * time_slice * directionY_ / direction;
  double next_posZ = positionZ_ + speed_ * time_slice * directionZ_ / direction;
  if (next_posX > map_sizeX || next_posX < 0) {
    directionX_ = -directionX_;
  }
  if (next_posY > map_sizeY || next_posY < 0) {
    directionY_ = -directionY_;
  }
  if (next_posZ > map_sizeZ || next_posZ < 0) {
    directionZ_ = -directionZ_;
  }
  positionX_ += speed_ * time_slice * directionX_ / direction;
  positionY_ += speed_ * time_slice * directionY_ / direction;
  positionZ_ += speed_ * time_slice * directionZ_ / direction;
}

// void BaseUnit::Update(int time_slice, map<int, BaseUnit> all_units){
//   // printf("Base Update\n");
// }

double BaseUnit::CalculateDistance(double x1, double y1, double z1, double x2, double y2, double z2) {
	double dx = x2 - x1;
	double dy = y2 - y1;
	double dz = z2 - z1;
	return sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
}

// 属性的getter方法
int BaseUnit::getId() const {
  return id_;
}

int BaseUnit::getCamp() const {
  return camp_;
}

bool BaseUnit::getStatus() const {
  return status_;
}

double BaseUnit::getPositionX() const {
  return positionX_;
}

double BaseUnit::getPositionY() const {
  return positionY_;
}

double BaseUnit::getPositionZ() const {
  return positionZ_;
}

double BaseUnit::getSize() const {
  return size_;
}

int BaseUnit::getPriority() const {
  return priority_;
}

int BaseUnit::getTargetId() const {
  return target_id_;
}

double BaseUnit::getTargetPositionX() const {
  return target_positionX_;
}

double BaseUnit::getTargetPositionY() const {
  return target_positionY_;
}

double BaseUnit::getTargetPositionZ() const {
  return target_positionZ_;
}

double BaseUnit::getDirectionX() const {
  return directionX_;
}

double BaseUnit::getDirectionY() const {
  return directionY_;
}

double BaseUnit::getDirectionZ() const {
  return directionZ_;
}

double BaseUnit::getSpeed() const {
  return speed_;
}

double BaseUnit::getMonitorDistance() const {
  return monitor_distance_;
}

double BaseUnit::getCommunicationDistance() const {
  return communication_distance_;
}

vector<int> BaseUnit::getMonitorObjects() const {
  return monitor_objects_;
}

vector<int> BaseUnit::getCommunicationObjects() const {
  return communication_objects_;
}

// 属性的setter方法
void BaseUnit::setCamp(int camp) {
  camp_ = camp;
}

void BaseUnit::setStatus(bool status) {
  status_ = status;
}

void BaseUnit::setPosition(double positionX, double positionY, double positionZ) {
  positionX_ = positionX;
  positionY_ = positionY;
  positionZ_ = positionZ;
}

void BaseUnit::setSize(double size) {
  size_ = size;
}

void BaseUnit::setPriority(int priority) {
  priority_ = priority;
}

void BaseUnit::setTargetId(int targetId) {
  target_id_ = targetId;
}

void BaseUnit::setTargetPosition(double targetPositionX, double targetPositionY, double targetPositionZ) {
  target_positionX_ = targetPositionX;
  target_positionY_ = targetPositionY;
  target_positionZ_ = targetPositionZ;
}

void BaseUnit::setDirection(double directionX, double directionY, double directionZ) {
  directionX_ = directionX;
  directionY_ = directionY;
  directionZ_ = directionZ;
}

void BaseUnit::setSpeed(double speed) {
  speed_ = speed;
}

void BaseUnit::setMonitorDistance(double distance) {
  monitor_distance_ = distance;
  communication_distance_ = distance;
}

void BaseUnit::setCommunicationDistance(double distance) {
  monitor_distance_ = distance;
  communication_distance_ = distance;
}

void BaseUnit::addMonitorObjects(int id) {
  if (find(monitor_objects_.begin(), monitor_objects_.end(), id) == monitor_objects_.end()) {
    monitor_objects_.push_back(id);
  }
} 

void BaseUnit::clearMonitorObjects() {
  monitor_objects_.clear();
  monitor_objects_.shrink_to_fit();
}  

void BaseUnit::deleteMonitorObjects(int id) {
  monitor_objects_.erase(remove(monitor_objects_.begin(), monitor_objects_.end(), id), monitor_objects_.end());
}

void BaseUnit::addCommunicationObjects(int id) {
  if (find(communication_objects_.begin(), communication_objects_.end(), id) == communication_objects_.end()) {
    communication_objects_.push_back(id);
  }
} 

void BaseUnit::clearCommunicationObjects() {
  communication_objects_.clear();
  communication_objects_.shrink_to_fit();
}  

void BaseUnit::deleteCommunicationObjects(int id) {
  communication_objects_.erase(remove(communication_objects_.begin(), communication_objects_.end(), id), communication_objects_.end());
}

double BaseUnit::getAttackRadius() const {
    return attack_radius_;
}

int BaseUnit::getWeaponNum() const {
    return weapon_num_;
}

void BaseUnit::setAttackRadius(double attackRadius) {
    attack_radius_ = attackRadius;
}

void BaseUnit::setWeaponNum(int weaponNum) {
    weapon_num_ = weaponNum;
}