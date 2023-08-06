#include "Utils.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>




// 计算两个点之间的欧几里德距离
double calculateDistance(double x1, double y1, double z1, double x2, double y2, double z2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    double dz = z2 - z1;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

// 找到距离Object最近的MapRegion对象并返回
const MapRegion& getMapBlockIndex(const BaseUnit& object, const map<int, MapRegion>& mapVector) {
    const double objectX = object.getPositionX();
    const double objectY = object.getPositionY();
    const double objectZ = object.getPositionZ();
    double minDistance = std::numeric_limits<double>::max();
    const MapRegion closestMapRegion = nullptr;

    for (const auto& mapRegion : mapVector) {
        double regionCenterX = mapRegion.second.getCenterX();
        double regionCenterY = mapRegion.second.getCenterY();
        double regionCenterZ = mapRegion.second.getCenterZ();

        double distance = calculateDistance(objectX, objectY, objectZ, regionCenterX, regionCenterY, regionCenterZ);

        if (distance < minDistance) {
            minDistance = distance;
            closestMapRegion = &mapRegion.second;
        }
    }
    closestMapRegion->addIncludeObjectID(object.getID());
    if (closestMapRegion == nullptr) {
        // Handle the case when the mapVector is empty or invalid.
        // You may throw an exception or return a default MapRegion object.
    }

    return closestMapRegion;
}

/*@required 所有实体的列表all_units，所有地图块的列表map_region_list)*/
/*@process 遍历所有的实体，根据实体类提供的方法判断该实体属于哪一块地图块，并将其加入地图块的关联列表中*/
/*@result 所有实体根据其最新位置，已加入到相应的地图块中的关联列表*/
/*@output null*/
void refreshMapRegionUnits(Map<int, BaseUnit> all_units, vector<MapRegion> map_region_list)
{
    for (int i = 0; i < map_region_list.size(); ++i) {
        //**此处clearIncludeIDs方法应在MapRegion类里添加，清空IncludeObjectIDs**
        map_block_list[i].clearIncludeObjectID();
    }
    for (int i = 0; i < all_units.size(); ++i) {
        //getMapBlockIndex方法可以作为基类的一个方法，在类内部根据类的位置信息去计算返回当前实体所处的地图号序号。
        MapRegion* return_map_block = getMapBlockIndex(all_units[i], map_region_list);
        //此处将一个实体加入某一块地图的关联列表
        return_map_block->addIncludeObjectID(all_units[i].getID());
    }
}


void InitRelatedObjectIDs(Map<int, BaseUnit> all_units) {
    for (int i = 0; i < all_units.size(); ++i) {
        for (int j = 0; j < all_units.size(); ++j) {
            double distance = calculateDistance(all_units[i].getPositionX(), all_units[i].getPositionY(), all_units[i].getPositionZ(),
                              all_units[j].getPositionX(), all_units[j].getPositionY(), all_units[j].getPositionZ());
            if (distance < CONNECTED_DISTANCE) {
                all_units[i].addRelatedObjectID(j);
            }         
        }
    }
}

void clearUnitConnectionList(Map<int, BaseUnit> all_units)
{
    for (int i = 0; i < all_units.size(); ++i) {
        //**此处clearRelatedObjectIDs方法应在BaseUnit类(每个种类的实体应该都有联系关系，应该可以作为基类属性)里添加，清空RelatedObjectIDs**
        for (int j = 0; j < all_units[i].getRelatedObjectIDs().size(); ++j) {
            BaseUnit[all_units[i].getRelatedObjectIDs()[j]].deleteRelatedObjectID(all_units[i].getID());
        }
        all_units[i].clearRelatedObjectID();
    }
}

bool connected(const BaseUnit& a, const BaseUnit& b)
{
    if (calculateDistance(a.getPositionX(), a.getPositionY(), a.getPositionZ(), 
                            b.getPositionX(), b.getPositionY(), b.getPositionZ()) <= CONNECTED_DISTANCE) {
        return true;
    } else {
        return false;
    }
}

void refresh_entity_connection(Map<int, BaseUnit> all_units, map<int, BaseUnit> cur_units, vector<MapRegion> map_region_list) {
    clearUnitConnectionList(cur_units);
    vector<int> unitsIDs;
    for (int i = 0; i < cur_units.size(); ++i) {
        unitsIDs.push_back(cur_units[i].getID());
    }
    for(int i = 0; i < map_region_list.size(); ++i) {
        MapRegion a_block = map_region_list[i];
        //获取到当前地图块内优先级为1的所有实体对象（**此处应给一个获取地图块内不同优先级实体的方法**）
        //此处get方法返回的是int数组，即实体的id，**此处若不修改类的定义则应提供一个id->object的转换函数**
        vector<BaseUnit> a_block_entity_list = a_block.getIncludeObjectIDs();
        //**此处需要MapRegion类根据块的类型，分别提供list0和list1**
        vector<int> related_map_block_list_0 = a_block.getRelatedMapUnitIDs();
        vector<int> related_map_block_list_1 = a_block.getSubRelatedMapUnitIDs();
        //计算最近相邻的地图块
        for (j = 0; j < related_map_block_list_0.size(); j++) {
            MapRegion b_block = map_region_list[related_map_block_list_0[j]];
            vector<int> b_block_entity_list = b_block.getIncludeObjectIDs();
            for (k = 0; k < a_block_entity_list.size(); k++) {
                if (find(unitsIDs.begin(), unitsIDs.end(), a_block_entity_list[k])) {
                    for (m = 0; m < b_block_entity_list.size(); m++) {
                        a_block_entity_list[k].addRelatedObjectID(b_block_entity_list[j]);
                        all_units[b_block_entity_list[j]].addRelatedObjectID(a_block_entity_list[k].getID());
                    }
                }
            }
        }

        //计算次最近相邻的地图块
        for (j = 0; j < related_map_block_list_1.size(); j++) {
            MapRegion b_block = map_region_list[related_map_block_list_0[j]];
            vector<int> b_block_entity_list = b_block.getIncludeObjectIDs();
            for (k = 0; k < a_block_entity_list.size(); k++) {
                if (find(unitsIDs.begin(), unitsIDs.end(), a_block_entity_list[k])) {
                    for (m = 0; m < b_block_entity_list.size(); m++) {
                        if (connected(a_block_entity_list[k], all_units[b_block_entity_list[j]])) {
                            a_block_entity_list[k].addRelatedObjectID(b_block_entity_list[j]);
                            all_units[b_block_entity_list[j]].addRelatedObjectID(a_block_entity_list[k].getID());
                        } 
                    }
                }
            }
        }
    }    
}

