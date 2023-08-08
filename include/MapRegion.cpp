#include "MapRegion.h"

// 构造函数
MapRegion::MapRegion(int id) 
  :id_(id) {
  centerX_ = 0;
  centerY_ = 0;
  centerZ_ = 0;
  radius_ = 1;
}

MapRegion::MapRegion(int id, double centerX, double centerY, double centerZ, double radius)
  :id_(id), centerX_(centerX), centerY_(centerY), centerZ_(centerZ), radius_(radius) {}

// 获取属性的方法
int MapRegion::getID() const {
  return id_;
}

double MapRegion::getCenterX() const {
  return centerX_;
}

double MapRegion::getCenterY() const {
  return centerY_;
}

double MapRegion::getCenterZ() const {
  return centerZ_;
}

double MapRegion::getRadius() const {
  return radius_;
}

const vector<int> MapRegion::getRelatedMapRegions() const {
  return related_map_regions_;
}

const vector<int> MapRegion::getSubRelatedMapRegions() const {
  return sub_related_map_regions_;
}

const vector<int> MapRegion::getIncludeUnits() const {
  return include_units_;
}

// 设置属性的方法
void MapRegion::addRelatedMapRegions(int map_region_id) {
  related_map_regions_.push_back(map_region_id);
}

void MapRegion::addSubRelatedMapRegions(int map_region_id) {
  sub_related_map_regions_.push_back(map_region_id);
}

void MapRegion::addIncludeUnits(int unit_id) {
  include_units_.push_back(unit_id);
}

void MapRegion::clearIncludeUnits() {
  include_units_.clear();
}
