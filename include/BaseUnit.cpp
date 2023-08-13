#include "BaseUnit.h"

// 构造函数
BaseUnit::BaseUnit(int id) :id_(id), 
                            camp_(0), 
                            status_(false), 
                            positionX_(0), 
                            positionY_(0), 
                            positionZ_(0),
                            size_(1), 
                            communication_distance_(0), 
                            directionX_(0), 
                            directionY_(0), 
                            directionZ_(0), 
                            speed_(0),
                            priority_(1) {}

BaseUnit::BaseUnit(int id, double positionX, double positionY, double positionZ):
                  id_(id), 
                  camp_(0), 
                  status_(false), 
                  positionX_(positionX), 
                  positionY_(positionY), 
                  positionZ_(positionZ),
                  size_(1), 
                  communication_distance_(0), 
                  directionX_(0), 
                  directionY_(0), 
                  directionZ_(0), 
                  speed_(0),
                  priority_(1) {}

BaseUnit::BaseUnit(int id, double speed, double directionX, double directionY, double directionZ, double positionX, double positionY, double positionZ) : 
                  id_(id), 
                  camp_(0), 
                  status_(false), 
                  positionX_(positionX), 
                  positionY_(positionY), 
                  positionZ_(positionZ),
                  size_(1), 
                  communication_distance_(0), 
                  directionX_(directionX), 
                  directionY_(directionY), 
                  directionZ_(directionZ), 
                  speed_(speed),
                  priority_(1) {}


// 移动对象的方法实现
void BaseUnit::move(double time_slice) {
    // 根据速度和运动方向更新位置
  double direction = sqrt(pow(directionX_, 2) + pow(directionY_, 2) + pow(directionZ_, 2));
  // printf("%f\n", direction);
  // printf("%f\n", speed_ * time_slice * directionX_ / direction);
  positionX_ += speed_ * time_slice * directionX_ / direction;
  // printf("%f\n", positionX_);
  positionY_ += speed_ * time_slice * directionY_ / direction;
  positionZ_ += speed_ * time_slice * directionZ_ / direction;
  // printf("%f %f %f\n", getPositionX(), getPositionY(), getPositionZ());
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

double BaseUnit::getCommunicationDistance() const {
  return communication_distance_;
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

int BaseUnit::getPriority() const {
  return priority_;
}

vector<int> BaseUnit::getRelatedObjects() const {
  return related_objects_;
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

void BaseUnit::setCommunicationDistance(double distance) {
  communication_distance_ = distance;
}

void BaseUnit::setDirection(double directionX, double directionY, double directionZ) {
  directionX_ = directionX;
  directionY_ = directionY;
  directionZ_ = directionZ;
  
}

void BaseUnit::setSpeed(double speed) {
  speed_ = speed;
}

void BaseUnit::setPriority(int priority) {
  priority_ = priority;
}

void BaseUnit::addRelatedObjects(int id) {
  if (find(related_objects_.begin(), related_objects_.end(), id) == related_objects_.end()) {
    related_objects_.push_back(id);
  }
} 

void BaseUnit::clearRelatedObjects() {
  related_objects_.clear();
}  

void BaseUnit::deleteRelatedObjects(int id) {
  for (auto iter = related_objects_.begin(); iter != related_objects_.end(); ++iter) {
    if (*iter == id) {
      related_objects_.erase(iter);
    }
  }
}