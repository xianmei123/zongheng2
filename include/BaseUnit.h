#ifndef BASEUNIT_H
#define BASEUNIT_H

#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

class BaseUnit {
public:
  // 构造函数
  BaseUnit(int id);
  BaseUnit(int id, double positionX, double positionY, double positionZ);
  BaseUnit(int id, double speed, double directionX, double directionY, double directionZ, double positionX, double positionY, double positionZ);

  // 改变速度
  void changeSpeed(double time_slice, double new_speed, double directionX, double directionY, double directionZ, double map_sizeX, double map_sizeY, double map_sizeZ);    // 改变对象速度的方法
  void move(double time_slice);  // 更新位置

  // 属性的getter和setter
  int getId() const;                             
  int getCamp() const;                           
  bool getStatus() const;
  double getPositionX() const;                   
  double getPositionY() const;                   
  double getPositionZ() const;         
  double getSize() const;
  double getCommunicationDistance() const;
  double getDirectionX() const;
  double getDirectionY() const;
  double getDirectionZ() const; 
  double getSpeed() const;    
  int getPriority() const;
  vector<int> getRelatedObjects() const; 
                       
  void setCamp(int camp);  
  void setStatus(bool status);
  void setPosition(double positionX, double positionY, double positionZ);   
  void setSize(double size);        
  void setCommunicationDistance(double distance);
  void setDirection(double directionX, double directionY, double directionZ);
  void setSpeed(double speed);
  void setPriority(int priority);
  void addRelatedObjects(int id);    
  void clearRelatedObjects();  
  void deleteRelatedObjects(int id); 

protected:
  int id_;                             // 单位ID
  int camp_;                           // 所属阵营
  bool status_;                        // 对象的状态（活动/死亡）
  double positionX_;                   // 位置坐标的X分量
  double positionY_;                   // 位置坐标的Y分量
  double positionZ_;                   // 位置坐标的Z分量
  double size_;                        // 单位大小
  double communication_distance_;      // 通讯距离
  double directionX_;                  // 运动方向的X分量
  double directionY_;                  // 运动方向的Y分量
  double directionZ_;                  // 运动方向的Z分量
  double speed_;                       // 速度
  int priority_;                       // 优先级
  vector<int> related_objects_;  // 与其关联的对象ID列表
};

#endif // BASEUNIT_H
