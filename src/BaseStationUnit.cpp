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

void BaseStationUnit::Update(int time_slice, map<int, shared_ptr<BaseUnit>> all_units, double *positions, double *target_positions, double *directions, double *distances, int *target_ids, int *status) {
    if (!status_) {
        return;
    }
    // printf("BaseStation update.\n");
    clearMonitorObjects();
    clearCommunicationObjects();
    int unit_num = all_units.size();
    for (unsigned int i = 0; i < all_units.size(); i++) {
        if (!status[i]) continue;
        auto unit_i = all_units.find(i)->second;
        if (unit_i->getCamp() == camp_) {
            if (distances[unit_num * (camp_ + 1) + i] < communication_distance_ && target_ids[i] == 0) {
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
    unsigned int size = monitor_objects_.size() > communication_objects_.size() ? communication_objects_.size() : monitor_objects_.size();
    for (unsigned int i = 0; i < size; i++) {
        target_ids[communication_objects_[i]] = monitor_objects_[i];
        target_positions[communication_objects_[i] * 3]     = positions[monitor_objects_[i] * 3];
        target_positions[communication_objects_[i] * 3 + 1] = positions[monitor_objects_[i] * 3 + 1];
        target_positions[communication_objects_[i] * 3 + 2] = positions[monitor_objects_[i] * 3 + 2];
    }
}