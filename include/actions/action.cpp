#include"action.h"



void randomAction(FlightUnit* unit_a,  FlightUnit* unit_b){
    //如果camp相同，则两个目标往相反的方向走
    if(unit_a->getCamp()!=unit_b->getCamp()){
        //获取当前unit_a于unit_b的speed和direction
        double speed_a=unit_a->getSpeed();
        double speed_b=unit_b->getSpeed();
        double direction_a_x=unit_a->getDirectionX();
        double direction_a_y=unit_a->getDirectionY();
        double direction_a_z=unit_a->getDirectionZ();
        double direction_b_x=unit_b->getDirectionX();
        double direction_b_y=unit_b->getDirectionY();
        double direction_b_z=unit_b->getDirectionZ();
        //分别翻转a与b的速度方向
        unit_a->changeSpeed(1,speed_a,-direction_a_x,-direction_a_y,-direction_a_z,1000,1000,1000);
        unit_b->changeSpeed(1,speed_b,-direction_b_x,-direction_b_y,-direction_b_z,1000,1000,1000);
    }
    //如果camp不相同，则随机摧毁其中一个目标
    else{
        //随机摧毁其中一个目标
        if(rand()%2==0){
            unit_a->setStatus(false);
        }
        else{
            unit_b->setStatus(false);
        }
    }

}