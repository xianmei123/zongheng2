#include "Utils.h"


// 生成num个随机数，最后一个元素是所有随机数的和
void GetRand(int num, int min, int max, vector<int>& rand_data) {
  int sum = 0;
  for (int i = 0; i < num; ++i) {
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
  // auto unit = all_units.find(0)->second;
  // printf("%d\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\t%.2f\t\t%.2f\t\t%.2f\n", unit.getId(),
  //                                                           unit.getPositionX(),
  //                                                           unit.getPositionY(),
  //                                                           unit.getPositionZ(),
  //                                                           unit.getSpeed(),
  //                                                           unit.getDirectionX(),
  //                                                           unit.getDirectionY(),
  //                                                           unit.getDirectionZ());
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

// 保存每次更新的状态, 为前端提供格式化数据 (by qjx)
void SaveAllUnitInfo(map<int, BaseUnit> all_units, int time_stamp) {
  using json = nlohmann::json;
  // Create JSON arrays for 'entity' and 'relation'
  json entity_array = json::array();
  json relation_array = json::array();
  for (unsigned i = 0; i < all_units.size(); ++i) {
    auto unit = all_units.find(i)->second;
    json entity_obj = {
      {"id", unit.getId()},
      {"posX", unit.getPositionX()},
      {"posY", unit.getPositionY()},
      {"posZ", unit.getPositionZ()},
      {"type", "Unit"}
    };
    entity_array.push_back(entity_obj);
    auto related_objects = unit.getRelatedObjects();
    for (auto related_id : related_objects) {
      json relation_obj = {
        {"entity1", unit.getId()},
        {"entity2", related_id}
      };
      relation_array.push_back(relation_obj);
    }
    json final_json = {
      {"entity", entity_array},
      {"relation", relation_array}
    };
     // Convert the JSON to a formatted string
    string json_str = final_json.dump(2); // 2-space indentation for readability
    // Create a file with a unique name based on the time_stamp
    string folder = "/home/luyao/zongheng/data/";
    string file_name = "unit_info_" + to_string(time_stamp) + ".json";
    // Write the JSON string to the file
    ofstream output_file;
    output_file.open(folder + file_name, ios::out);
    if (!output_file.is_open()) {
      cerr << "Failed to open file for writing: " << file_name << std::endl;
    } else {
      output_file << json_str;
      output_file.close();
      // cout << "JSON data saved to " << file_name << std::endl;
    }
  }
}

void PrintUnitRelated(map<int, BaseUnit> all_units) {
  for (unsigned i = 0; i < all_units.size(); ++i) {
    printf("Currently, the %d unit's related units are:\n", i);
    auto related_id =  all_units.find(i)->second.getRelatedObjects();
    for (unsigned j = 0; j < related_id.size(); ++j) {
      printf("%d ", related_id[j]);
    }
    printf("\n");
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

void PrintRegions(map<int, MapRegion> all_regions) {
  printf("--------------------------------------------------------\n");
  printf("Start initial map region...\n");
  printf("ID\tcenterX\tcenterY\tcenterZ\tradius\n");
  for (unsigned i = 0; i < all_regions.size(); ++i) {
    printf("%d\t%.2f\t%.2f\t%.2f\t%.2f\n", all_regions.find(i)->second.getID(),
                                            all_regions.find(i)->second.getCenterX(),
                                            all_regions.find(i)->second.getCenterY(),
                                            all_regions.find(i)->second.getCenterZ(),
                                            all_regions.find(i)->second.getRadius());
    auto related_id = all_regions.find(i)->second.getRelatedMapRegions();
    printf("The region %d related regions:", i);
    for (unsigned j = 0; j < related_id.size(); ++j) {
      printf("%d ", related_id[j]);
    } 
    printf("\nThe region %d sub related regions:", i);    
    auto sub_related_id = all_regions.find(i)->second.getSubRelatedMapRegions();
    for (unsigned j = 0; j < sub_related_id.size(); ++j) {
      printf("%d ", sub_related_id[j]);
    } 
    printf("\n");
  }
}

// 该map返回一个map，key是需要进行改变的时刻，该key对应的vector中包含该时刻需要改变的对象id，以及该对象的新速度和方向
map<int, map<int, vector<int>>> GetChangeTime(int rand_kind, int time_length, int num, map<int, BaseUnit> all_units) {
  map<int, map<int, vector<int>>> change_times;
  if (rand_kind == 1) { // 均匀分布
    vector<int> time_stamps;
    GetRand(num, 1, time_length, time_stamps);
    sort(time_stamps.begin(), time_stamps.end());
    vector<int> ids;
	  GetRand(num, 0, all_units.size(), ids);
    vector<int> new_speed;
    GetRand(num, 0, 150, new_speed);
    vector<int> new_direction;
    GetRand(2 * num, 0, 10, new_direction);
    map<int, vector<int>> change_objs;
    int cur_time_stamp = time_stamps[0];
    for (int i = 0; i < num - 1; ++i) {
      vector<int> new_speed_and_direction;
      if (all_units.find(ids[i])->second.getPriority() == 1) {
        new_speed_and_direction.push_back(50 + new_speed[i] % 150);
      } else if (all_units.find(ids[i])->second.getPriority() == 2) {
        new_speed_and_direction.push_back(10 + new_speed[i] % 40);
      } else if (all_units.find(ids[i])->second.getPriority() == 3) {
        new_speed_and_direction.push_back(new_speed[i] % 10);
      } else {
        new_speed_and_direction.push_back(0);
      }
      new_speed_and_direction.push_back(new_direction[i]);
      new_speed_and_direction.push_back(new_direction[i + num]);
      new_speed_and_direction.push_back(0);
      change_objs[ids[i]] = new_speed_and_direction;
      if (cur_time_stamp != time_stamps[i + 1]) {
        if (i == num - 2) break;
        change_times[cur_time_stamp] = change_objs;
        cur_time_stamp = time_stamps[i + 1];
        change_objs.clear();
      }
    }
    vector<int> new_speed_and_direction;
    if (all_units.find(ids[num - 1])->second.getPriority() == 1) {
      new_speed_and_direction.push_back(50 + new_speed[num - 1] % 150);
    } else if (all_units.find(ids[num - 1])->second.getPriority() == 2) {
      new_speed_and_direction.push_back(10 + new_speed[num - 1] % 40);
    } else if (all_units.find(ids[num - 1])->second.getPriority() == 3) {
      new_speed_and_direction.push_back(new_speed[num - 1] % 10);
    } else {
      new_speed_and_direction.push_back(0);
    }
    new_speed_and_direction.push_back(new_direction[num - 1]);
    new_speed_and_direction.push_back(new_direction[2 * num - 1]);
    new_speed_and_direction.push_back(0);
    if (cur_time_stamp == time_stamps[num - 1]) {
      change_objs[ids[num - 1]] = new_speed_and_direction;
      change_times[cur_time_stamp] = change_objs;
    } else {
      change_times[cur_time_stamp] = change_objs;
      change_objs.clear();
      change_objs[ids[num - 1]] = new_speed_and_direction;
      change_times[time_stamps[num - 1]] = change_objs;
    }
  }
  return change_times;
}

void PrintChangeTime(map<int, map<int, vector<int>>> change_times) {
  for (auto ite = change_times.begin(); ite != change_times.end(); ++ite) {
    printf("The change time stamp is: %d\n", ite->first);
    auto change_objs = ite->second;
    for (auto iter = change_objs.begin(); iter != change_objs.end(); ++iter) {
      printf("the unit id: %d, the new speed is: %d\n", iter->first, iter->second[0]);
      printf("The new direction is: %d, %d, %d\n", iter->second[1], iter->second[2], iter->second[3]);
    }
  }
}

// 计算两个单位之间的欧几里德距离
double CalculateDistance(double x1, double y1, double z1, double x2, double y2, double z2) {
	double dx = x2 - x1;
	double dy = y2 - y1;
	double dz = z2 - z1;
	return sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
}

// 找到距离unit最近的MapRegion，将该unit添加到该region的包含列表中
void GetMapRegion(BaseUnit& unit, map<int, MapRegion>& map_regions) {
	double unit_positionX = unit.getPositionX();
	double unit_positionY = unit.getPositionY();
	double unit_positionZ = unit.getPositionZ();
	double min_distance = numeric_limits<double>::max();
	int region_id = 0;

  for (unsigned i = 0; i < map_regions.size(); ++i) {
    auto map_region = map_regions.find(i)->second;
    auto distance = CalculateDistance(unit_positionX, unit_positionY, unit_positionZ,
                                      map_region.getCenterX(), map_region.getCenterY(), map_region.getCenterZ());
    if (distance < min_distance) {
      min_distance = distance;
      region_id = i;
    }                              
  }
	map_regions.find(region_id)->second.addIncludeUnits(unit.getId());
}

// 更新每个区域内的unit
void RefreshMapRegionUnits(map<int, BaseUnit>& all_units, map<int, MapRegion>& all_regions) {
  for (unsigned i = 0; i < all_regions.size(); ++i) {
    all_regions.find(i)->second.clearIncludeUnits();
  }
  for (unsigned i = 0; i < all_units.size(); ++i) {
    GetMapRegion(all_units.find(i)->second, all_regions);
  }
}

void PrintMapRegionUnits(map<int, MapRegion> all_regions) {
  for (unsigned i = 0; i < all_regions.size(); ++i) {
    printf("The %d region include unit:", i);
    auto include_units = all_regions.find(i)->second.getIncludeUnits();
    for (auto unit : include_units) {
      printf("%d ", unit);
    }
    printf("\n");
  }
}

void ChangeSpeed(BaseUnit &unit, double time_slice, double new_speed, double directionX, double directionY, double directionZ, double map_sizeX, double map_sizeY, double map_sizeZ) {
  double direction = sqrt(pow(directionX, 2) + pow(directionY, 2) + pow(directionZ, 2));
  double next_posX = unit.getPositionX() + new_speed * time_slice * directionX / direction;
  double next_posY = unit.getPositionY() + new_speed * time_slice * directionY / direction;
  double next_posZ = unit.getPositionZ() + new_speed * time_slice * directionZ / direction;
  if (next_posX > map_sizeX || next_posX < 0) {
    directionX = -directionX;
  }
  if (next_posY > map_sizeY || next_posY < 0) {
    directionY = -directionY;
  }
  if (next_posZ > map_sizeZ || next_posZ < 0) {
    directionZ = -directionZ;
  }
  unit.setDirection(directionX, directionY, directionZ);
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
void InitRelatedUnits(map<int, BaseUnit>& all_units) {
  for (unsigned i = 0; i < all_units.size(); ++i) {
    for (unsigned j = 0; j < all_units.size(); ++j) {
      if (IsConnected(all_units.find(i)->second, all_units.find(j)->second) && i != j) {
        // printf("%d %d\n", i ,j);
        all_units.find(i)->second.addRelatedObjects(j);
      }
    }
  }
}

// 初始化所有region的相关region列表：
void InitRelatedRegions(map<int, MapRegion>& all_regions, int radius) {
  for (unsigned i = 0; i < all_regions.size(); ++i) {
    for (unsigned j = 0; j < all_regions.size(); ++j) {
      double center_dis = CalculateDistance(all_regions.find(i)->second.getCenterX(), all_regions.find(i)->second.getCenterY(), all_regions.find(i)->second.getCenterZ(),
                                            all_regions.find(j)->second.getCenterX(), all_regions.find(j)->second.getCenterY(), all_regions.find(j)->second.getCenterZ());
      if (center_dis <= CONNECTED_DISTANCE - 2 * radius) {
				all_regions.find(i)->second.addRelatedMapRegions(j);
			} else if (center_dis <= CONNECTED_DISTANCE + 2 * radius) {
				all_regions.find(i)->second.addSubRelatedMapRegions(j);
			} 
    }
  }
}

// 清除units的连接关系，并删除所有与之关联的unit的列表中该unit的ID
void ClearRelatedObjects(vector<int> cur_ids, map<int, BaseUnit>& all_units) {
  for (unsigned i = 0; i < cur_ids.size(); ++i) {
    auto unit = all_units.find(cur_ids[i])->second;
    // printf(",,,,\n");
    for (unsigned j = 0; j < unit.getRelatedObjects().size(); ++j) {
      // printf("%d ", unit.getRelatedObjects()[j]);
      all_units.find(unit.getRelatedObjects()[j])->second.deleteRelatedObjects(cur_ids[i]);
    } 
    // printf("\n");
    all_units.find(cur_ids[i])->second.clearRelatedObjects();
    // for (auto related_obj : unit.getRelatedObjects()) {
    //   printf("%d ", related_obj);
    // }
    // printf("\n");
  }
}

// 更新需要更新的优先级的unit的连接关系，按区域进行，第一个参数即需要更新的节点
void RefreshUnitsRelated(vector<int> cur_ids, map<int, BaseUnit>& all_units, map<int, MapRegion> all_regions) {
	ClearRelatedObjects(cur_ids, all_units);
  // printf("%p\n", &(all_units.find(1)->second));
  // for (unsigned i = 0; i < cur_ids.size(); ++i) {
  //   auto unit = all_units.find(cur_ids[i])->second;
  //   // printf(",,,,\n");
  //   for (unsigned j = 0; j < unit.getRelatedObjects().size(); ++j) {
  //     // printf("%d ", unit.getRelatedObjects()[j]);
  //     all_units.find(unit.getRelatedObjects()[j])->second.deleteRelatedObjects(cur_ids[i]);
  //   } 
  //   // printf("\n");
  //   unit.clearRelatedObjects();
  //   if (i == 1) {
  //     // for (auto related_obj : unit.getRelatedObjects()) {
  //     //   printf("*%d ", related_obj);
  //     // }
  //     printf("unit ptr %p\n", &unit);
  //   }
  // }

  // auto unit = all_units.find(cur_ids[0])->second;
  // printf("related_objs ptr %p\n",  &unit);
  // for (auto related_obj : unit.getRelatedObjects()) {
  //   printf("%d ", related_obj);
  // }
  // printf("\n");
  for (unsigned i = 0; i < all_regions.size(); ++i) {
    auto region_units = all_regions.find(i)->second.getIncludeUnits();// 得到当前区域包含unit
		auto related_regions = all_regions.find(i)->second.getRelatedMapRegions();	// 得到全相关区域id
		auto sub_related_regions = all_regions.find(i)->second.getSubRelatedMapRegions();	// 得到半相关区域id
    for (auto region_unit : region_units) {	//遍历当前区域所有unit
			if (find(cur_ids.begin(), cur_ids.end(), region_unit) != cur_ids.end()) {	// 说明该区域中有需要更新的节点
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