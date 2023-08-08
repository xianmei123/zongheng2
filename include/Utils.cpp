#include "Utils.h"

// 生成num个随机数，最后一个元素是所有随机数的和
void GetRand(int num, int min, int max, vector<int>& rand_data) {
  int sum = 0;
  for (int i = 0; i < num; ++i) {
    // 生成1~10的随机数
    int cur_data = min + rand() % max;
    rand_data.push_back(cur_data);
    sum += cur_data;
  }
  rand_data.push_back(sum);
}

// 初始化所有unit
void InitAllUnits(vector<int> units, map<int, BaseUnit>& all_units, int map_size_row, int map_size_col) {
  printf("Start init all units...\n");
  int cur_id = 0; // 已经添加的对象个数
  for (int id = 0; id < units[0]; ++id) {  // 速度50~199
    all_units.emplace(id, FlightUnit(id, 50 + rand() % 150, rand() % 10, rand() % 10, 0, 
                                    rand() % map_size_row, rand() % map_size_col, 0));
    all_units.find(id)->second.setPriority(1);
  }
  cur_id += units[0];
  for (int id = 0; id < units[1]; ++id) {  // 速度10~49
    all_units.emplace(id + cur_id, FlightUnit(id + cur_id, 10 + rand() % 40, rand() % 10, rand() % 10, 0, 
                                              rand() % map_size_row, rand() % map_size_col, 0));
    all_units.find(id + cur_id)->second.setPriority(2);
  }
  cur_id += units[1];
  for (int id = 0; id < units[2]; ++id) {  // 速度0~9
    all_units.emplace(id + cur_id, GroundUnit(id + cur_id, rand() % 10, rand() % 10, rand() % 10, 0,
                                              rand() % map_size_row, rand() % map_size_col, 0));
    all_units.find(id + cur_id)->second.setPriority(3);
  }
  cur_id += units[2];
  for (int id = 0; id < units[3]; ++id) {
    all_units.emplace(id + cur_id, NonMovableUnit(id + cur_id, rand() % map_size_row, rand() % map_size_col, 0));
    all_units.find(id + cur_id)->second.setPriority(4);
  }
}

void PrintAllUnit(map<int, BaseUnit> all_units, int time_stamp) {
  printf("--------------------------------------------------------\n");
  printf("Current time stamp: %d\n", time_stamp);
  printf("ID\tpositionX\tpositionY\tpositionZ\tspeed\tdirectionX\tdirectionY\tdirectionZ\n");
  for (auto unit : all_units) {
    printf("%d\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\t%.2f\t\t%.2f\t\t%.2f\n", unit.second.getId(),
                                                            unit.second.getPositionX(),
                                                            unit.second.getPositionY(),
                                                            unit.second.getPositionZ(),
                                                            unit.second.getSpeed(),
                                                            unit.second.getDirectionX(),
                                                            unit.second.getDirectionY(),
                                                            unit.second.getDirectionZ());
  }
}
// 初始化所有map region
void InitAllRegions(map<int, MapRegion>& all_regions, int map_size_row, int map_size_col, int radius) {
  int region_num = 0;
  for (int i = 0; i < map_size_row / (radius * 2); ++i) {
    for (int j = 0; j < map_size_col / (radius * 2); ++j) {
      all_regions.emplace(region_num, MapRegion(region_num, i * (radius * 2) + radius, j * (radius * 2) + radius, 0, radius));
      region_num ++;
    }
  }
}

// 得到新的速度和方向
vector<int> GetNewSpeedAndDirection(int id, map<int, BaseUnit> all_units) {
  vector<int> new_speed_and_direction;
  if (all_units.find(id)->second.getPriority() == 1) {
    new_speed_and_direction.push_back(50 + rand() % 150);
  } else if (all_units.find(id)->second.getPriority() == 2) {
    new_speed_and_direction.push_back(10 + rand() % 40);
  } else if (all_units.find(id)->second.getPriority() == 3) {
    new_speed_and_direction.push_back(rand() % 10);
  } else {
    return new_speed_and_direction;
  }
  new_speed_and_direction.insert(new_speed_and_direction.end(), {rand() % 10, rand() % 10, 0});

  return new_speed_and_direction;
}

// 该map返回一个map，key是需要进行改变的时刻，该key对应的vector中包含该时刻需要改变的对象id，以及该对象的新速度和方向
map<int, vector<int>> GetChangeTime(int rand_kind, int time, int num, int unit_num, map<int, BaseUnit> all_units) {
	map<int, vector<int>> change_times;
  if (rand_kind == 1) { // 均匀分布
    vector<int> time_stamps;
    GetRand(num, 0, time, time_stamps);
    sort(time_stamps.begin(), time_stamps.end());
	  vector<int> id_new_speed_and_direction;
    for (int i = 0; i < num; ++i) {
      int change_id = rand() % (unit_num);
      id_new_speed_and_direction.push_back(change_id);
      vector<int> new_speed_and_direction = GetNewSpeedAndDirection(change_id, all_units);
      id_new_speed_and_direction.insert(id_new_speed_and_direction.end(), new_speed_and_direction.begin(), new_speed_and_direction.end());
      change_times[time_stamps[i]] = id_new_speed_and_direction;
    }
  }
  return change_times;
}


// 计算两个单位之间的欧几里德距离
double CalculateDistance(double x1, double y1, double z1, double x2, double y2, double z2) {
	double dx = x2 - x1;
	double dy = y2 - y1;
	double dz = z2 - z1;
	return sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
}

// 找到距离unit最近的MapRegion，将该unit添加到该region的包含列表中
void GetMapRegion(BaseUnit unit, map<int, MapRegion> map_regions) {
	double unit_positionX = unit.getPositionX();
	double unit_positionY = unit.getPositionY();
	double unit_positionZ = unit.getPositionZ();
	double min_distance = numeric_limits<double>::max();
	int region_id = 0;

	for (auto map_region : map_regions) {
		double region_centerX = map_region.second.getCenterX();
		double region_centerY = map_region.second.getCenterY();
		double region_centerZ = map_region.second.getCenterZ();
		auto distance = CalculateDistance(unit_positionX, unit_positionY, unit_positionZ,
																			region_centerX, region_centerY, region_centerZ);
		if (distance < min_distance) {
			min_distance = distance;
			region_id = map_region.first;
		}
	}
	map_regions.find(region_id)->second.addIncludeUnits(unit.getId());
}

// 更新每个区域内的unit
void RefreshMapRegionUnits(map<int, BaseUnit> all_units, map<int, MapRegion> all_regions) {
	for (auto region : all_regions) {
		region.second.clearIncludeUnits();
	}
	for (auto unit : all_units) {
		GetMapRegion(unit.second, all_regions);
	}
}

// 判断两个unit是否连接
bool IsConnected(BaseUnit unit_fir, BaseUnit unit_sec) {
	if (CalculateDistance(unit_fir.getPositionX(), unit_fir.getPositionY(), unit_fir.getPositionZ(), 
												unit_sec.getPositionX(), unit_sec.getPositionY(), unit_sec.getPositionZ()) <= CONNECTED_DISTANCE) {
		return true;
	} else {
		return false;
	}
}

// 初始化所有unit的相关unit列表
void InitRelatedUnits(map<int, BaseUnit> all_units) {
	for (auto unit_fir : all_units) {
		for (auto unit_sec : all_units) {
			if (IsConnected(unit_fir.second, unit_sec.second) && unit_fir.first != unit_sec.first) {
				unit_fir.second.addRelatedObjects(unit_sec.first);
			}
		}
	}
}

// 初始化所有region的相关region列表：
void InitRelatedRegions(map<int, MapRegion> all_regions, int radius) {
	for (auto region_fir : all_regions) {
		for (auto region_sec : all_regions) {
			double distance = CalculateDistance(region_fir.second.getCenterX(), region_fir.second.getCenterY(), region_fir.second.getCenterZ(), 
                                          region_sec.second.getCenterX(), region_sec.second.getCenterY(), region_sec.second.getCenterZ());
      if (distance <= CONNECTED_DISTANCE - 2 * radius) {
				region_fir.second.addRelatedMapRegions(region_sec.first);
			} else if (distance <= CONNECTED_DISTANCE + 2 * radius) {
				region_fir.second.addSubRelatedMapRegions(region_sec.first);
			} 
		}
	}
}
// 清除units的连接关系，并删除所有与之关联的unit的列表中该unit的ID
void ClearRelatedObjects(map<int, BaseUnit> units, map<int, BaseUnit> all_units) {
	for (auto unit : units) {
		for (auto id : unit.second.getRelatedObjects()) {
			all_units.find(id)->second.deleteRelatedObjects(unit.first);
		}
		unit.second.clearRelatedObjects();
	}
}

// 更新需要更新的优先级的unit的连接关系，按区域进行，第一个参数即需要更新的节点
void RefreshUnitsRelated(map<int, BaseUnit> units, map<int, BaseUnit> all_units, map<int, MapRegion> all_regions) {
	ClearRelatedObjects(units, all_units);
	for (auto region : all_regions) {	// 遍历所有区域
		auto region_units = region.second.getIncludeUnits();	// 得到当前区域包含unit
		auto related_regions = region.second.getRelatedMapRegions();	// 得到全相关区域id
		auto sub_related_regions = region.second.getSubRelatedMapRegions();	// 得到半相关区域id
		for (auto region_unit : region_units) {	//遍历当前区域所有unit
			if (units.find(region_unit) != units.end()) {	// 说明该区域中有需要更新的节点
				for (auto related_region : related_regions) {	// 首先遍历全相关区域
					for (auto include_unit : all_regions.find(related_region)->second.getIncludeUnits()) {	// 遍历该区域内的节点
						if (include_unit != region_unit) {	// 保证不把自己添加到相关节点中
							all_units.find(region_unit)->second.addRelatedObjects(include_unit);	// 添加互相之间的连接关系
							all_units.find(include_unit)->second.addRelatedObjects(region_unit);
						}
					}
				}
				for (auto sub_related_region : sub_related_regions) {	// 接着遍历半相关区域
					for (auto include_unit : all_regions.find(sub_related_region)->second.getIncludeUnits()) {	// 遍历该区域内的节点
						if (IsConnected(all_units.find(region_unit)->second, all_units.find(include_unit)->second)) {	// 满足距离限制
							all_units.find(region_unit)->second.addRelatedObjects(include_unit);	// 添加互相之间的连接关系
							all_units.find(include_unit)->second.addRelatedObjects(region_unit);
						}
					}
				}
			}
		}
	}
}