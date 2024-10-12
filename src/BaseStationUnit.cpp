#include "BaseStationUnit.h"
#include "PlaneUnit.h"
#include "MissleUnit.h"

void BaseStationUnit::Init(int region_start_x, int region_start_y, int region_size_x, int region_size_y, int map_size_z) {
    priority_ = 4;
    monitor_distance_ = CONNECTED_DISTANCE;
    communication_distance_ = CONNECTED_DISTANCE;
    weapon_num_ = 0;
    attack_radius_ = 0;
}

void BaseStationUnit::Update(int time_slice, map<int, shared_ptr<BaseUnit>> all_units, double *positions, double *target_positions, double *directions, double *distances, int *target_ids) {
    if (!status_) {
        return;
    }
    // printf("BaseStation update.\n");
    clearMonitorObjects();
    clearCommunicationObjects();
    int unit_num = all_units.size();
    for (unsigned int i = 0; i < all_units.size(); i++) {
        auto unit_i = all_units.find(i)->second;
        if (!unit_i->getStatus()) continue;
        if (unit_i->getCamp() == camp_) {
            if (distances[unit_num * (camp_ + 1) + i] < communication_distance_ && unit_i->getTargetId() == 0) {
                if (auto plUnit = std::dynamic_pointer_cast<PlaneUnit>(unit_i)) {
                    addCommunicationObjects(i);
                }
            }
        } else {
            if (distances[unit_num * (camp_ + 1) + i] < monitor_distance_) {
                addMonitorObjects(i);
            }
        }
    }
    // 监测到敌方单位，寻找己方的飞行器进行拦截，目前是如果飞行器没有目标才拦截
    for (unsigned int i = 0; i < monitor_objects_.size(); i++) {
        if (i >= communication_objects_.size()) break;

        auto unit_mon = all_units.find(monitor_objects_[i])->second; // 敌方单位
        auto unit_com = all_units.find(communication_objects_[i])->second; // 己方飞行器

        unit_com->setTargetId(monitor_objects_[i]);
        target_ids[communication_objects_[i]] = monitor_objects_[i];
        target_positions[communication_objects_[i] * 3] = unit_mon->getPositionX();
        target_positions[communication_objects_[i] * 3 + 1] = unit_mon->getPositionY();
        target_positions[communication_objects_[i] * 3 + 2] = unit_mon->getPositionZ();
    }
}
    //for (unsigned int i = 0; i < monitor_objects_.size(); i++) {
    //    auto unit_i = all_units.find(monitor_objects_[i])->second;
    //    for (unsigned int j = 0; j < communication_objects_.size(); j++) {
    //        auto unit_j = all_units.find(communication_objects_[j])->second;
    //        if (unit_j->getTargetId() == 0) { // 没有目标id
    //            if (auto plUnit = std::dynamic_pointer_cast<PlaneUnit>(unit_j)) {
    //                printf("test0\n");
    //                unit_j->setTargetId(monitor_objects_[i]);
    //                target_ids[communication_objects_[j]] = monitor_objects_[i];
    //                target_positions[communication_objects_[j]*3]     = unit_i->getPositionX();
    //                target_positions[communication_objects_[j]*3 + 1] = unit_i->getPositionY();
    //                target_positions[communication_objects_[j]*3 + 2] = unit_i->getPositionZ();
    //                break;
    //            }
    //        }
    //    }
    //}
}