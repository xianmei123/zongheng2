#include "MissleUnit.h"

void MissleUnit::Init(int region_start_x, int region_start_y, int region_size_x, int region_size_y, int map_size_z) {
    priority_ = 1;
    monitor_distance_ = CONNECTED_DISTANCE;
    communication_distance_ = CONNECTED_DISTANCE;
    weapon_num_ = 1;
    attack_radius_ = CONNECTED_DISTANCE - 100;
    target_positionX_ = region_start_x + rand() % region_size_x;
    target_positionY_ = region_start_y + rand() % region_size_y;
    target_positionZ_ = rand() % map_size_z;
}

void MissleUnit::Update(int time_slice, map<int, shared_ptr<BaseUnit>> all_units, double *positions, double *target_positions, double *directions, double *distances, int *target_ids, int* status) {
    if (!status_) {
        return;
    }
    // printf("Missle update.\n");
    positionX_ = positions[id_*3];
    positionY_ = positions[id_*3 + 1];
    positionZ_ = positions[id_*3 + 2];
    if (target_id_ != 0) {
        auto target_unit = all_units.find(target_id_)->second;
        double targetX = target_unit->getPositionX();
        double targetY = target_unit->getPositionY();
        double targetZ = target_unit->getPositionZ();
        if (distances[id_] < attack_radius_) {
            target_unit->setStatus(false);
            status_ = false;
        } else { // 不符合攻击条件，继续朝着目标飞行
            directionX_ = targetX - positionX_;
            directionY_ = targetY - positionY_;
            directionZ_ = targetZ - positionZ_;
        }
    } else if (target_positionX_ != 0 || target_positionY_ !=0 || target_positionZ_ != 0) {
        // 检查距离
        if (distances[id_] < attack_radius_) {
            status_ = false;
        } else { // 不符合攻击条件，继续朝着目标飞行
            directionX_ = target_positionX_ - positionX_;
            directionY_ = target_positionY_ - positionY_;
            directionZ_ = target_positionZ_ - positionZ_;
        }
    }
    directions[id_*3]     = directionX_;
    directions[id_*3 + 1] = directionY_;
    directions[id_*3 + 2] = directionZ_;
}