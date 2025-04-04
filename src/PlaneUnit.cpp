#include "PlaneUnit.h"

double PlaneUnit::getHitProbability() const {
    return hit_probability_;
}

void PlaneUnit::setHitProbability(double hitProbability) {
    hit_probability_ = hitProbability;
}

void PlaneUnit::Init(int region_start_x, int region_start_y, int region_size_x, int region_size_y, int map_size_z) {
    priority_ = 1;
    monitor_distance_ = CONNECTED_DISTANCE;
    communication_distance_ = CONNECTED_DISTANCE;
    attack_radius_ = CONNECTED_DISTANCE - 100;
    weapon_num_ = 3 + rand() % 2; 
    hit_probability_ = 0.6;
    target_positionX_ = region_start_x + rand() % region_size_x;
    target_positionY_ = region_start_y + rand() % region_size_y;
    target_positionZ_ = rand() % map_size_z;
}

void PlaneUnit::Update(int time_slice, map<int, shared_ptr<BaseUnit>> all_units, double *positions, double *target_positions, double *directions, double *distances, int *target_ids, int* status) {
    if (!status_) {
        return;
    }
    // printf("Plane update.\n");
    positionX_ = positions[id_*3];
    positionY_ = positions[id_*3 + 1];
    positionZ_ = positions[id_*3 + 2];
    if (target_id_ != 0) {
        // 检查弹药和距离
        auto target_unit = all_units.find(target_id_)->second;
        double targetX = target_unit->getPositionX();
        double targetY = target_unit->getPositionY();
        double targetZ = target_unit->getPositionZ();
        if (distances[id_] < attack_radius_) {
            // 攻击
            if (weapon_num_ > 0) weapon_num_--;
            // 命中后死亡
            if (((((double)rand())) / RAND_MAX) < hit_probability_) {
                target_unit->setStatus(false);
            }
            target_id_ = 0;
            target_positions[id_*3]     = target_positionX_;
            target_positions[id_*3 + 1] = target_positionY_;
            target_positions[id_*3 + 2] = target_positionZ_;
            // 随机生成方向
            directionX_ = rand() % 10;
            directionY_ = rand() % 10;
            directionZ_ = 0;
        } else { // 攻击失败或距离不够无法攻击，继续朝着目标飞行
            directionX_ = targetX - positionX_;
            directionY_ = targetY - positionY_;
            directionZ_ = targetZ - positionZ_;
        }
    } else if (target_positionX_ != 0 || target_positionY_ !=0 || target_positionZ_ != 0) {
        // 检查距离
        if (distances[id_] < attack_radius_) {
            if (weapon_num_ > 0) weapon_num_--;
            target_positionX_ = 0;
            target_positionY_ = 0;
            target_positionZ_ = 0;
            // 随机生成方向
            directionX_ = rand() % 10;
            directionY_ = rand() % 10;
            directionZ_ = 0;

        } else { // 继续朝着目标飞行
            directionX_ = target_positionX_ - positionX_;
            directionY_ = target_positionY_ - positionY_;
            directionZ_ = target_positionZ_ - positionZ_;
        }
    } else {
        // 随机飞
        directionX_ = rand() % 10;
        directionY_ = rand() % 10;
        directionZ_ = 0;
    }
    directions[id_*3]     = directionX_;
    directions[id_*3 + 1] = directionY_;
    directions[id_*3 + 2] = directionZ_;
}