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
void InitAllUnits(vector<int> units, map<int, shared_ptr<BaseUnit>>& all_units, int map_size_x, int map_size_y, int map_size_z, int camp, int unit_num,
    int* my_position, int* enemy_position) {
  printf("Start init all units for camp: %d...\n", camp);
  static int cur_id = 0; // 已经添加的对象个数
  for (int id = 0; id < units[0]; ++id) {  // 基站，优先级4
    auto newUnit = make_shared<BaseStationUnit>(id + cur_id, my_position[0] + rand() % my_position[2], my_position[1] + rand() % my_position[3], 0);
    newUnit->Init(enemy_position[0], enemy_position[1], enemy_position[2], enemy_position[3], map_size_z);
    newUnit->setCamp(camp);
    all_units.emplace(id + cur_id, newUnit);
  }
  cur_id += units[0];
  for (int id = 0; id < units[1]; ++id) {  // 飞行器，速度300-800，优先级1
    auto newUnit = make_shared<PlaneUnit>(id + cur_id, 300 + rand() % 500, rand() % 10, rand() % 10, 0, my_position[0] + rand() % my_position[2], my_position[1] + rand() % my_position[3], rand() % map_size_z);
    newUnit->Init(enemy_position[0], enemy_position[1], enemy_position[2], enemy_position[3], map_size_z);
    newUnit->setCamp(camp);
    all_units.emplace(id + cur_id, newUnit);
  }
  cur_id += units[1];
  for (int id = 0; id < units[2] / 2; ++id) {  // 导弹，速度800-1500，优先级1，打击目标地点
    auto newUnit = make_shared<MissleUnit>(id + cur_id, 800 + rand() % 700, rand() % 10, rand() % 10, 0, my_position[0] + rand() % my_position[2], my_position[1] + rand() % my_position[3], rand() % map_size_z);
    newUnit->Init(enemy_position[0], enemy_position[1], enemy_position[2], enemy_position[3], map_size_z);
    newUnit->setCamp(camp);
    all_units.emplace(id + cur_id, newUnit);
  }
  for (int id = units[2] / 2; id < units[2]; ++id) {  // 导弹，速度800-1500，优先级1，打击目标
    auto newUnit = make_shared<MissleUnit>(id + cur_id, 800 + rand() % 700, rand() % 10, rand() % 10, 0, my_position[0] + rand() % my_position[2], my_position[1] + rand() % my_position[3], rand() % map_size_z);
    newUnit->Init(enemy_position[0], enemy_position[1], enemy_position[2], enemy_position[3], map_size_z);
    newUnit->setCamp(camp);
    if (camp == 0) {
      newUnit->setTargetId(unit_num + rand() % unit_num);
    } else {
      newUnit->setTargetId(rand() % unit_num);
    } 
    all_units.emplace(id + cur_id, newUnit);
  }
  cur_id += units[2];
  for (int id = 0; id < units[3]; ++id) {  // 坦克，速度10-30，优先级2
    auto newUnit = make_shared<TankUnit>(id + cur_id, 10 + rand() % 20, rand() % 10, rand() % 10, 0, my_position[0] + rand() % my_position[2], my_position[1] + rand() % my_position[3], 0);
    newUnit->Init(enemy_position[0], enemy_position[1], enemy_position[2], enemy_position[3], map_size_z);
    newUnit->setCamp(camp);
    all_units.emplace(id + cur_id, newUnit);
  }
  cur_id += units[3];
  for (int id = 0; id < units[4]; ++id) {  // 士兵，速度1-3，优先级3
    auto newUnit = make_shared<SoldierUnit>(id + cur_id, 1 + rand() % 2, rand() % 10, rand() % 10, 0, my_position[0] + rand() % my_position[2], my_position[1] + rand() % my_position[3], 0);
    newUnit->Init(enemy_position[0], enemy_position[1], enemy_position[2], enemy_position[3], map_size_z);
    newUnit->setCamp(camp);
    all_units.emplace(id + cur_id, newUnit);
  }
  cur_id += units[4];
}

void PrintAllUnit(map<int, shared_ptr<BaseUnit>> all_units, int time_stamp) {
  vector<int> camp_num(2);
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
    if (unit.second->getStatus()) {
      printf("%d\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\t%.2f\t\t%.2f\t\t%.2f\n", unit.second->getId(),
                                                            unit.second->getPositionX(),
                                                            unit.second->getPositionY(),
                                                            unit.second->getPositionZ(),
                                                            unit.second->getSpeed(),
                                                            unit.second->getDirectionX(),
                                                            unit.second->getDirectionY(),
                                                            unit.second->getDirectionZ());
      camp_num[unit.second->getCamp()]++;
    }
  }
  printf("Camp_0 alive: %d\tCamp_1 alive: %d\n", camp_num[0], camp_num[1]);
}

void PrintAllUnit(map<int, shared_ptr<BaseUnit>> all_units, int time_stamp, double *positions, double *directions, int *status) {
  vector<int> camp_num(2);
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
    int id = unit.second->getId();
    if (status[id]) {
      printf("%d\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\t%.2f\t\t%.2f\t\t%.2f\n", unit.second->getId(),
                                                            positions[id * 3],
                                                            positions[id * 3 + 1],
                                                            positions[id * 3 + 2],
                                                            unit.second->getSpeed(),
                                                            directions[id * 3],
                                                            directions[id * 3 + 1],
                                                            directions[id * 3 + 2]);
      camp_num[unit.second->getCamp()]++;
    }
  }
  printf("Camp_0 alive: %d\tCamp_1 alive: %d\n", camp_num[0], camp_num[1]);
}

// 保存每次更新的状态, 为前端提供格式化数据 (by qjx)
//void SaveAllUnitInfo(map<int, shared_ptr<BaseUnit>> all_units, int time_stamp) {
//  using json = nlohmann::json;
//  // Create JSON arrays for 'entity' and 'relation'
//  json entity_array = json::array();
//  json relation_array = json::array();
//  for (unsigned i = 0; i < all_units.size(); ++i) {
//    auto unit = all_units.find(i)->second;
//    json entity_obj = {
//      {"id", unit->getId()},
//      {"posX", unit->getPositionX()},
//      {"posY", unit->getPositionY()},
//      {"posZ", unit->getPositionZ()},
//      {"type", "Unit"}
//    };
//    entity_array.push_back(entity_obj);
//    auto related_objects = unit->getMonitorObjects();
//    for (auto related_id : related_objects) {
//      json relation_obj = {
//        {"entity1", unit->getId()},
//        {"entity2", related_id}
//      };
//      relation_array.push_back(relation_obj);
//    }
//    json final_json = {
//      {"entity", entity_array},
//      {"relation", relation_array}
//    };
//     // Convert the JSON to a formatted string
//    string json_str = final_json.dump(2); // 2-space indentation for readability
//    // Create a file with a unique name based on the time_stamp
//    string folder = "/data/luyao/luyao/zongheng/data/";
//    string file_name = "unit_info_" + to_string(time_stamp) + ".json";
//    // Write the JSON string to the file
//    ofstream output_file;
//    output_file.open(folder + file_name, ios::out);
//    if (!output_file.is_open()) {
//      cerr << "Failed to open file for writing: " << file_name << std::endl;
//    } else {
//      output_file << json_str;
//      output_file.close();
//      // cout << "JSON data saved to " << file_name << std::endl;
//    }
//  }
//}

void PrintUnitRelated(map<int, shared_ptr<BaseUnit>> all_units) {
  for (unsigned i = 0; i < all_units.size(); ++i) {
    printf("Currently, the %d unit's related units are:\n", i);
    auto related_id =  all_units.find(i)->second->getMonitorObjects();
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

// 该map返回一个map，key是需要进行改变的时刻，该key对应的vector中包含该时刻需要改变的对象id，以及该对象的新速度
map<int, map<int, double>> GetChangeTime(int rand_kind, int time_length, int num, map<int, shared_ptr<BaseUnit>> all_units) {
  map<int, map<int, double>> change_times;
  if (rand_kind == 1) { // 均匀分布
    vector<int> time_stamps; // 更新速度的时间戳
    GetRand(num, 1, time_length, time_stamps);
    sort(time_stamps.begin(), time_stamps.end());
    vector<int> ids;
	  GetRand(num, 0, all_units.size(), ids);
    vector<int> ran_speed;
    GetRand(num, 0, 700, ran_speed);
    map<int, double> change_speed;
    int cur_time_stamp = time_stamps[0];
    for (int i = 0; i < num - 1; ++i) {
      double new_speed = 0;
      auto unit_i = all_units.find(ids[i])->second;
      if (unit_i->getPriority() == 1) {
        if (auto plUnit = dynamic_pointer_cast<PlaneUnit>(unit_i)) {
          new_speed = 300 + ran_speed[i] % 500;
        }
        else {
          new_speed = 800 + ran_speed[i] % 700;
        }
      } else if (unit_i->getPriority() == 2) {
        new_speed = 10 + ran_speed[i] % 20;
      } else if (unit_i->getPriority() == 3) {
        new_speed = 1 + ran_speed[i] % 2;
      } else {
        new_speed = 0;
      }
      change_speed[ids[i]] = new_speed;
      if (cur_time_stamp != time_stamps[i + 1]) {
        if (i == num - 2) break;
        change_times[cur_time_stamp] = change_speed;
        cur_time_stamp = time_stamps[i + 1];
        change_speed.clear();
      }
    }
    double new_speed = 0;
    auto unit_i = all_units.find(ids[num - 1])->second;
    if (unit_i->getPriority() == 1) {
      if (auto plUnit = dynamic_pointer_cast<PlaneUnit>(unit_i)) {
        new_speed = 300 + ran_speed[num - 1] % 500;
      }
      else {
        new_speed = 800 + ran_speed[num - 1] % 700;
      }
    } else if (unit_i->getPriority() == 2) {
      new_speed = 10 + ran_speed[num - 1] % 20;
    } else if (unit_i->getPriority() == 3) {
      new_speed = 1 + ran_speed[num - 1] % 2;
    } else {
      new_speed = 0;
    }
    if (cur_time_stamp == time_stamps[num - 1]) {
      change_speed[ids[num - 1]] = new_speed;
      change_times[cur_time_stamp] = change_speed;
    } else {
      change_times[cur_time_stamp] = change_speed;
      change_speed.clear();
      change_speed[ids[num - 1]] = new_speed;
      change_times[time_stamps[num - 1]] = change_speed;
    }
  }
  return change_times;
}
/*
map<int, map<int, vector<int>>> GetChangeTime(int rand_kind, int time_length, int num, map<int, BaseUnit> all_units) {
  map<int, map<int, vector<int>>> change_times;
  if (rand_kind == 1) { // 均匀分布
    vector<int> time_stamps;
    GetRand(num, 1, time_length, time_stamps);
    sort(time_stamps.begin(), time_stamps.end());
    vector<int> ids;
	  GetRand(num, 0, all_units.size(), ids);
    vector<int> new_speed;
    GetRand(num, 0, 700, new_speed);
    vector<int> new_direction;
    GetRand(2 * num, 0, 10, new_direction);
    map<int, vector<int>> change_objs;
    int cur_time_stamp = time_stamps[0];
    for (int i = 0; i < num - 1; ++i) {
      vector<int> new_speed_and_direction;
      if (all_units.find(ids[i])->second.getPriority() == 1) {
        if (typeid(all_units.find(ids[i])->second) == typeid(PlaneUnit)) {
          new_speed_and_direction.push_back(300 + new_speed[i] % 500);
        }
        else {
          new_speed_and_direction.push_back(800 + new_speed[i] % 700);
        }
      } else if (all_units.find(ids[i])->second.getPriority() == 2) {
        new_speed_and_direction.push_back(10 + new_speed[i] % 20);
      } else if (all_units.find(ids[i])->second.getPriority() == 3) {
        new_speed_and_direction.push_back(1 + new_speed[i] % 2);
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
      if (typeid(all_units.find(ids[num - 1])->second) == typeid(PlaneUnit)) {
        new_speed_and_direction.push_back(300 + new_speed[num - 1] % 500);
      }
      else {
        new_speed_and_direction.push_back(800 + new_speed[num - 1] % 700);
      }
    } else if (all_units.find(ids[num - 1])->second.getPriority() == 2) {
      new_speed_and_direction.push_back(10 + new_speed[num - 1] % 20);
    } else if (all_units.find(ids[num - 1])->second.getPriority() == 3) {
      new_speed_and_direction.push_back(1 + new_speed[num - 1] % 2);
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
*/

void PrintChangeTime(map<int, map<int, double>> change_times) {
  for (auto ite = change_times.begin(); ite != change_times.end(); ++ite) {
    printf("The change time stamp is: %d\n", ite->first);
    auto change_objs = ite->second;
    for (auto iter = change_objs.begin(); iter != change_objs.end(); ++iter) {
      printf("the unit id: %d, the new speed is: %f\n", iter->first, iter->second);
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
void GetMapRegion(shared_ptr<BaseUnit>& unit, map<int, MapRegion>& map_regions) {
	double unit_positionX = unit->getPositionX();
	double unit_positionY = unit->getPositionY();
	double unit_positionZ = unit->getPositionZ();
	double min_distance = DBL_MAX;
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
	map_regions.find(region_id)->second.addIncludeUnits(unit->getId());
}

// 更新每个区域内的unit
void RefreshMapRegionUnits(map<int, shared_ptr<BaseUnit>>& all_units, map<int, MapRegion>& all_regions) {
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
  unit.setSpeed(new_speed);
  unit.setDirection(directionX, directionY, directionZ);
}

// 判断两个unit是否连接
bool IsConnected(shared_ptr<BaseUnit> unit_fir, shared_ptr<BaseUnit> unit_sec) {
	if (CalculateDistance(unit_fir->getPositionX(), unit_fir->getPositionY(), unit_fir->getPositionZ(), 
												unit_sec->getPositionX(), unit_sec->getPositionY(), unit_sec->getPositionZ()) <= min(unit_fir->getCommunicationDistance(), unit_sec->getCommunicationDistance())) {
		return true;
	} else {
		return false;
	}
}

// 初始化所有unit的相关unit列表
void InitRelatedUnits(map<int, shared_ptr<BaseUnit>>& all_units) {
  for (unsigned i = 0; i < all_units.size(); ++i) {
    auto unit_i = all_units.find(i)->second;
    for (unsigned j = 0; j < all_units.size(); ++j) {
      auto unit_j = all_units.find(j)->second;
      if (IsConnected(unit_i, unit_j) && i != j) {
        // printf("%d %d\n", i ,j);
        unit_i->addMonitorObjects(j);
        unit_i->addCommunicationObjects(j);
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
void ClearRelatedObjects(vector<int> cur_ids, map<int, shared_ptr<BaseUnit>>& all_units) {
  for (unsigned i = 0; i < cur_ids.size(); ++i) {
    auto unit = all_units.find(cur_ids[i])->second;
    // printf(",,,,\n");
    for (unsigned j = 0; j < unit->getMonitorObjects().size(); ++j) {
      // printf("%d ", unit.getRelatedObjects()[j]);
      all_units.find(unit->getMonitorObjects()[j])->second->deleteMonitorObjects(cur_ids[i]);
    } 
    // printf("\n");
    all_units.find(cur_ids[i])->second->clearMonitorObjects();
    // for (auto related_obj : unit.getRelatedObjects()) {
    //   printf("%d ", related_obj);
    // }
    // printf("\n");

    for (unsigned j = 0; j < unit->getCommunicationObjects().size(); ++j) {
      all_units.find(unit->getCommunicationObjects()[j])->second->deleteCommunicationObjects(cur_ids[i]);
    } 
    all_units.find(cur_ids[i])->second->clearCommunicationObjects();
  }
}

// 更新需要更新的优先级的unit的连接关系，按区域进行，第一个参数即需要更新的节点 TODO:Monitor&Communication
void RefreshUnitsRelated(vector<int> cur_ids, map<int, shared_ptr<BaseUnit>>& all_units, map<int, MapRegion> all_regions) {
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
							all_units.find(region_unit)->second->addMonitorObjects(include_unit);	// 添加互相之间的连接关系
							all_units.find(include_unit)->second->addMonitorObjects(region_unit);
						}
					}
				}
				for (auto sub_related_region : sub_related_regions) {	// 接着遍历半相关区域
					for (auto include_unit : all_regions.find(sub_related_region)->second.getIncludeUnits()) {	// 遍历该区域内的节点
						if (IsConnected(all_units.find(region_unit)->second, all_units.find(include_unit)->second)) {	// 满足距离限制
							all_units.find(region_unit)->second->addMonitorObjects(include_unit);	// 添加互相之间的连接关系
							all_units.find(include_unit)->second->addMonitorObjects(region_unit);
						}
					}
				}
			}
		}
	}
}

void ProduceData(queue<DataChunk>& dataChunkBuffer, mutex& dataChunkMutex, Vertex *vertices, int *indices, int unit_num) {
  Vertex* tmp_vertices = new Vertex[unit_num];
  int* tmp_indices = new int[unit_num];
  memcpy(tmp_vertices, vertices, sizeof(Vertex) * unit_num);
  memcpy(tmp_indices, indices, sizeof(int) * unit_num);
  DataChunk newDataChunk = { tmp_vertices, tmp_indices, unit_num };
  static int count = 0;
  std::unique_lock<std::mutex> lock(dataChunkMutex); // 锁定互斥量
  dataChunkBuffer.push(newDataChunk);
  // std::cout << "Produced a DataChunk: " << count++ << std::endl;
}

void Log1Print(ofstream &log_file, double *positions, int unit_num) {
  map<int, vector<int>> grid_units;
  for (unsigned int id = 0; id < unit_num; id++) {
    unsigned int grid_num = floor(positions[id*3])  * 10000 + floor(positions[id*3+1]);
    if (grid_units.find(grid_num) == grid_units.end()) {
      vector<int> newVec;
      grid_units.emplace(grid_num, newVec);
    }
    grid_units.find(grid_num)->second.push_back(id);
  }

  map<int, vector<int>>::iterator iter;
  for (iter = grid_units.begin(); iter != grid_units.end(); ++iter) {
      log_file << "Grid: " + to_string(iter->first) << endl;
      string grid_unit("Unit:");
      for (unsigned int i = 0; i < iter->second.size(); i++) {
        grid_unit.append(" ");
        grid_unit.append(to_string(iter->second.at(i)));
      }
      log_file << grid_unit << endl;
  }
  
  cout << "Log1Print Over." << endl;
}

void Log2Print(ofstream &log_file, double *positions, double *directions, int *status, int unit_num) {
  log_file << "ID\tpositionX\tpositionY\tpositionZ\tstatus" << endl;
  for (unsigned int id = 0; id < unit_num; id++) {
    log_file << to_string(id) + "\t" + to_string(positions[id*3]) + "\t" + to_string(positions[id*3+1]) + "\t"
              + to_string(positions[id*3+2]) + "\t" + to_string(status[id]) << endl;
  }
  cout << "Log2Print Over." << endl;
}

void Log3Print(ofstream &log_file, int time_stamp, int *status, int *unit_class, int unit_num, double run_time) {
  int camp_0[5] = {0, 0, 0, 0, 0};
  int camp_1[5] = {0, 0, 0, 0, 0};
  int alive_0 = 0, alive_1 = 0;
  
  for (unsigned int i = 0; i < unit_num / 2; i++) {
    if (status[i]) {
      alive_0++;
      camp_0[unit_class[i]]++;
    }
  }

  for (unsigned int i = unit_num / 2; i < unit_num; i++) {
    if (status[i]) {
      alive_1++;
      camp_1[unit_class[i]]++;
    }
  }

  log_file << "Step:\t" + to_string(time_stamp) << endl;
  log_file << "Runtime:\t" + to_string(run_time) + "s"<< endl;
  log_file << "Unit\t\tCamp0\t\tCamp1" << endl;
  log_file << "BaseStation\t"   + to_string(camp_0[0]) + "\t\t\t" + to_string(camp_1[0]) << endl;
  log_file << "Plane\t\t"       + to_string(camp_0[1]) + "\t\t"   + to_string(camp_1[1]) << endl;
  log_file << "Missle\t\t"      + to_string(camp_0[2]) + "\t\t"   + to_string(camp_1[2]) << endl;
  log_file << "Tank\t\t"        + to_string(camp_0[3]) + "\t\t"   + to_string(camp_1[3]) << endl;
  log_file << "Soldier\t\t"     + to_string(camp_0[4]) + "\t\t"   + to_string(camp_1[4]) << endl;
  log_file << "Alive\t\t"       + to_string(alive_0)   + "\t\t"   + to_string(alive_1)   << endl;
  log_file << " " << endl;
}

void Log4Print(ofstream &log_file, queue<int>& queryIdBuffer, mutex& queryIdMutex, map<int, shared_ptr<BaseUnit>>& all_units, int* unit_class, double *positions, double *directions, int *status, int *weapon_nums) {
  std::unique_lock<std::mutex> lock(queryIdMutex);
  unsigned int size = queryIdBuffer.size();
  for(unsigned int i = 0; i < size; i++) {
      int id = queryIdBuffer.front();
      auto unit_id = all_units.find(id)->second;
      queryIdBuffer.pop();
      log_file << "Unit:\t" + to_string(id) << endl;
      log_file << "Camp:\t" + to_string(unit_id->getCamp()) << endl;
      log_file << "Status:\t" + to_string(status[id]) << endl;
      log_file << "Position:\t" + to_string(positions[id*3]) + ", " + to_string(positions[id*3+1]) + ", " + to_string(positions[id*3+2]) << endl;
      log_file << "Direction:\t" + to_string(directions[id*3]) + ", " + to_string(directions[id*3+1]) + ", " + to_string(directions[id*3+2]) << endl;
      log_file << "Speed:\t" + to_string(unit_id->getSpeed()) << endl;
      log_file << "Priority:\t" + to_string(unit_id->getPriority()) << endl;
      log_file << "AttackRadius:\t" + to_string(unit_id->getAttackRadius()) << endl;
      log_file << "WeaponNum:\t" + to_string(weapon_nums[id]) << endl;
      log_file << " " << endl;
   } 
}

void Log5Print(ofstream &log_file, int time_stamp) {
  PROCESS_MEMORY_COUNTERS pmc;
  if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
  {
    log_file << "Step:\t" + to_string(time_stamp) << endl;
    log_file << "当前进程占用内存大小为：" + to_string(pmc.WorkingSetSize / 1024 / 1024)  + "MB" << endl;
  }
}