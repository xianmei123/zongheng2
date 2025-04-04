#ifndef BASEUNIT_H
#define BASEUNIT_H

#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <map>
#include <typeinfo>
#include <memory>

using namespace std;

#define CONNECTED_DISTANCE 500

class BaseUnit {
public:
  // 构造函数
  BaseUnit(int id);
  BaseUnit(int id, double positionX, double positionY, double positionZ);
  BaseUnit(int id, double speed, double directionX, double directionY, double directionZ, double positionX, double positionY, double positionZ);

  // 改变速度
  // void changeSpeed(double time_slice, double new_speed, double directionX, double directionY, double directionZ, double map_sizeX, double map_sizeY, double map_sizeZ);    // 改变对象速度的方法
  void move(double time_slice, double map_sizeX, double map_sizeY, double map_sizeZ);  // 更新位置
  virtual void Update(int time_slice, map<int, shared_ptr<BaseUnit>> all_units, double *positions, double *target_positions, double *directions, double *distances, int *target_ids, int *status) = 0;

  virtual ~BaseUnit() = default; // 确保Base类是多态的

  double CalculateDistance(double x1, double y1, double z1, double x2, double y2, double z2);

  // 属性的getter和setter
  int getId() const;                             
  int getCamp() const;                           
  bool getStatus() const;
  double getPositionX() const;                   
  double getPositionY() const;                   
  double getPositionZ() const;         
  double getSize() const;
  int getPriority() const;
  int getTargetId() const;
  double getTargetPositionX() const;
  double getTargetPositionY() const;
  double getTargetPositionZ() const;
  double getDirectionX() const;
  double getDirectionY() const;
  double getDirectionZ() const; 
  double getSpeed() const;   
  double getMonitorDistance() const; 
  double getCommunicationDistance() const; 
  vector<int> getMonitorObjects() const; 
  vector<int> getCommunicationObjects() const; 
                       
  void setCamp(int camp);  
  void setStatus(bool status);
  void setPosition(double positionX, double positionY, double positionZ);   
  void setSize(double size);   
  void setPriority(int priority);
  void setTargetId(int targetId);
  void setTargetPosition(double targetPositionX, double targetPositionY, double targetPositionZ);
  void setDirection(double directionX, double directionY, double directionZ);
  void setSpeed(double speed);
  void setMonitorDistance(double distance);
  void setCommunicationDistance(double distance);
  void addMonitorObjects(int id);    
  void clearMonitorObjects();  
  void deleteMonitorObjects(int id); 
  void addCommunicationObjects(int id);    
  void clearCommunicationObjects();  
  void deleteCommunicationObjects(int id); 

  double getAttackRadius() const;
  int getWeaponNum() const;
  void setAttackRadius(double attackRadius);
  void setWeaponNum(int weaponNum);

protected:
  int id_;                             // 单位ID
  int camp_;                           // 所属阵营
  bool status_;                        // 对象的状态（活动/死亡）
  double positionX_;                   // 位置坐标的X分量
  double positionY_;                   // 位置坐标的Y分量
  double positionZ_;                   // 位置坐标的Z分量
  double size_;                        // 单位大小
  int priority_;                       // 优先级
  int target_id_;                      // 目标ID
  double target_positionX_;            // 目标坐标的X分量
  double target_positionY_;            // 目标坐标的Y分量
  double target_positionZ_;            // 目标坐标的Z分量
  double directionX_;                  // 运动方向的X分量
  double directionY_;                  // 运动方向的Y分量
  double directionZ_;                  // 运动方向的Z分量
  double speed_;                       // 速度
  int weapon_num_;
  double attack_radius_;
  double monitor_distance_;            // 检测距离
  double communication_distance_;      // 通讯距离
  vector<int> monitor_objects_;        // 监测ID列表
  vector<int> communication_objects_;  // 通讯ID列表
};

#endif // BASEUNIT_H
