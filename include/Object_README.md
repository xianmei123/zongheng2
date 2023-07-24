# 对象定义设计文档

## 飞行单位类
<!-- ![Alt text](军用飞机分类.png) -->
**描述：** 用作所有飞行单位的基类。

- 属性：
  - id, int
  - camp, int 所属阵营
  - status, bool 状态
  - X, Y, Z, double 位置坐标
  - speed, double  速度
  - directionX, directionY, directionZ, double 运动方向
  - size, double 单位大小
  - communication_distance, double, 通讯距离
  - relatedObjectIDs, array<int> 关联的对象ID列表
- 方法：
  - void move(double timeSlice) 控制对象移动, 改变对象位置
  - void change_speed() 改变速度和运动方向  
- 飞行单位包括：
  - 运输机
  - 战斗机
    - 歼击机
    - 侦察机
    - 轰炸机
  - 教练机
  - 直升机
    - 武装直升机
    - 反潜直升机
    - 运输直升机
  - 无人机
  - 特种飞机
    - 预警机
    - 电子对抗飞机
    - 反潜机
  - 加油机
  - 飞行武器
    - 导弹
    - 火箭弹
 
## 固定单位类
- 属性
  - 和飞行单位一致，去掉速度和运动方向
- 方法
- 固定单位包括（重要的军用设施）
  - 固定雷达
  - 机场
  - 导弹发射井
  - 指挥部
  - 部队驻扎/集结点
  - 港口
  - 发电站
## 地面单位类
- 属性
  - 和飞行单位一致
- 方法
  - 和飞行单位一致
- 地面单位包括
  - 车辆
  - 舰船   
  - 人
## 地图区域类
- 属性
  - ID, int 
  - centerX, centerY, centerZ, double 区域中心
  - radius, double 半径, 值为到任意一个端点的长度（规则的多边形）
  - relatedMapUnitIDs, array<int> 两个块的中心距离小于通讯距离
  - includeObjectIDs, array<int>, 区域中包含的对象ID
- 方法
  - 