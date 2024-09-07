#include "SoldierUnit.h"

void SoldierUnit::Init(int map_size_x, int map_size_y, int map_size_z) {
    priority_ = 3;
    monitor_distance_ = CONNECTED_DISTANCE;
    communication_distance_ = CONNECTED_DISTANCE;
    attack_radius_ = CONNECTED_DISTANCE - 400;
    weapon_num_ = 5 + rand() % 5; 
    target_positionX_ = rand() % map_size_x;
    target_positionY_ = rand() % map_size_y;
    target_positionZ_ = 0;
    init_positionX_ = positionX_;
    init_positionY_ = positionY_;
    init_positionZ_ = positionZ_;
}

void SoldierUnit::Update(int time_slice, map<int, shared_ptr<BaseUnit>> all_units, double *positions, double *target_positions, double *directions, double *distances, int *target_ids) {
    if (!status_) {
        return;
    }
    // printf("Soldier update.\n");
    positionX_ = positions[id_*3];
    positionY_ = positions[id_*3 + 1];
    positionZ_ = positions[id_*3 + 2];
    if (target_positionX_ != 0 || target_positionY_ !=0 || target_positionZ_ != 0) {
        if (distances[id_] < attack_radius_) {
            if (weapon_num_ > 0) weapon_num_--;
            target_positionX_ = 0;
            target_positionY_ = 0;
            target_positionZ_ = 0;
            // 原路返回
            directionX_ = init_positionX_ - positionX_;
            directionY_ = init_positionY_ - positionY_;
            directionZ_ = init_positionZ_ - positionZ_;
        } else {
            directionX_ = target_positionX_ - positionX_;
            directionY_ = target_positionY_ - positionY_;
            directionZ_ = target_positionZ_ - positionZ_;
        }
    }
    directions[id_*3]     = directionX_;
    directions[id_*3 + 1] = directionY_;
    directions[id_*3 + 2] = directionZ_;
}