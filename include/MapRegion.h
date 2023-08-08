#ifndef MAPREGION_H
#define MAPREGION_H

#include <vector>

using namespace std;
class MapRegion {
public:
  // 构造函数
  MapRegion(int id);
  MapRegion(int id, double centerX, double centerY, double centerZ, double radius);

  // 获取属性的方法
  int getID() const;                      // 获取区域ID
  double getCenterX() const;              // 获取区域中心X坐标
  double getCenterY() const;              // 获取区域中心Y坐标
  double getCenterZ() const;              // 获取区域中心Z坐标
  double getRadius() const;               // 获取区域半径
  const vector<int> getRelatedMapRegions() const;    // 获取相关地图单元ID数组
  const vector<int> getSubRelatedMapRegions() const;    // 获取相关地图单元ID数组
  const vector<int> getIncludeUnits() const;     // 获取包含对象ID数组
  
  // 设置属性的方法
  void addRelatedMapRegions(int map_region_id);    // 添加相关地图单元ID
  void addSubRelatedMapRegions(int map_region_id); // 添加半相关地图单元ID数组
  void addIncludeUnits(int unit_id);      // 添加包含对象ID
  void clearIncludeUnits();

private:
  int id_;                               // 区域ID
  double centerX_;                       // 区域中心X坐标
  double centerY_;                       // 区域中心Y坐标
  double centerZ_;                       // 区域中心Z坐标
  double radius_;                        // 区域半径
  vector<int> related_map_regions_;   // 相关地图单元ID数组
  vector<int> sub_related_map_regions_; // 半相关地图单元ID数组
  vector<int> include_units_;    // 包含对象ID数组
};

#endif // MAPREGION_H
