/*please include some header files*/
#include <iostream>
#include "BaseUnit.h"
#include "FlightUnit.h"
#include "NonMovableUnit.h"
#include "MapRegion.h"
#include "GroundUnit.h"
#include "Utils.h"

using namespace std;

//设置两个实体距离小于多少时，二者可以建立联系
#define CONNECTED_DISTANCE 100.00

//使用BaseUnit作为单位，使用基类
vector<BaseUnit> entity_list;

//MapRegion作为地图块类
vector<MapRegion> map_block_list;

//时间戳记录
double TimeStamp = 0.0;


void clear_map_include_list()
{
    int i = 0;
    for (i = 0; i < map_block_list.size(); i++) {
        //**此处clearIncludeIDs方法应在MapRegion类里添加，清空IncludeObjectIDs**
        map_block_list.at(i).clearIncludeIDs();
    }
}

/*@required 所有实体的列表(暂时命名为entity_list)，所有地图块的列表(暂时命名为map_block_list)*/
/*@process 遍历所有的实体，根据实体类提供的方法判断该实体属于哪一块地图块，并将其加入地图块的关联列表中*/
/*@result 所有实体根据其最新位置，已加入到相应的地图块中的关联列表*/
/*@output null*/
void refresh_map_block_list()
{
    int i = 0;
    //在刷新之前，应将所有地图块的包含列表全部清空
    clear_map_include_list();
    for (i = 0; i < entity_list.size(); i++) {
        //getMapBlockIndex方法可以作为基类的一个方法，在类内部根据类的位置信息去计算返回当前实体所处的地图号序号。
        MapRegion return_map_block = getMapBlockIndex(entity_list.at(i), map_block_list);
        //此处将一个实体加入某一块地图的关联列表
        return_map_block.addIncludeObjectID(entity_list.at(i).getID());
    }
}

void clear_entity_connection_list()
{
    int i = 0;
    for (i = 0; i < entity_list.size(); i++) {
        //**此处clearRelatedObjectIDs方法应在BaseUnit类(每个种类的实体应该都有联系关系，应该可以作为基类属性)里添加，清空RelatedObjectIDs**
        entity_list.at(i).clearRelatedObjectIDs();
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

double getTimeStamp()
{
    return TimeStamp;
}

//该方法利用地图块顺序的遍历方式进行更新计算，还可以考虑利用实体遍历的方式计算（n^2）
/*@required 所有地图块的列表(暂时命名为map_block_list)*/
/*@process 先按地图块的ID从小到大遍历，将当前地图块中的实体与其中的列表0和列表1的地图块内的实体进行通讯连接计算和更新，按照优先级的顺序进行*/
/*@result 每个实体类内部的relatedObjectIDs列表根据当前位置更新为最新的内容*/
/*@output null*/
void refresh_entity_connection()
{
    //最开始应将全部连接清空
    clear_entity_connection_list();
    int i = 0;
    int j = 0;
    int k = 0;
    int m = 0;
    //优先计算优先级为1
    for(i = 0; i < map_block_list.size(); i++) {
        MapRegion a_block = map_block_list.at(i);
        //获取到当前地图块内优先级为1的所有实体对象（**此处应给一个获取地图块内不同优先级实体的方法**）
        //此处get方法返回的是int数组，即实体的id，**此处若不修改类的定义则应提供一个id->object的转换函数**
        vector<BaseUnit> a_block_entity_list = a_block.getRelatedMapUnitIDs();
        //**此处需要MapRegion类根据块的类型，分别提供list0和list1**
        vector<int> related_map_block_list_0 = a_block.getRelatedMapUnitIDs0();
        vector<int> related_map_block_list_1 = a_block.getRelatedMapUnitIDs1();
        //计算最近相邻的地图块
        for (j = 0; j < related_map_block_list_0.size(); j++) {
            MapRegion b_block = //获取到related_map_block_list_0中的一个地图块类
            //获取到当前地图块内所有的实体对象（所有优先级的对象）
            //此处get方法返回的是int数组，即实体的id，**此处若不修改类的定义则应提供一个id->object的转换函数**
            vector<BaseUnit> b_block_entity_list = b_block.getIncludeObjectIDs();

            for (k = 0; k < a_block_entity_list.size(); k++) {
                BaseUnit now_entity = a_block_entity_list.at(k);
                //因为此处的优先级为1，1s更新一次，而时间片的大小也为1s，因此不用计算是否整除
                for (m = 0; m < b_block_entity_list.size(); m++) {
                    now_entity.addRelatedObjectID(b_block_entity_list.at(m));
                }
            }
        }

        //计算次最近相邻的地图块
        for (j = 0; j < related_map_block_list_1.size(); j++) {
            MapRegion b_block = //获取到related_map_block_list_1中的一个地图块类
            //获取到当前地图块内所有的实体对象（所有优先级的对象）
            vector<BaseUnit> b_block_entity_list = b_block.getIncludeObjectIDs();

            for (k = 0; k < a_block_entity_list.size(); k++) {
                FlightUnit now_entity = a_block_entity_list.at(k);
                //因为此处的优先级为1，1s更新一次，而时间片的大小也为1s，因此不用计算是否整除
                for (m = 0; m < b_block_entity_list.size(); m++) {
                    if (connected(now_entity, b_block_entity_list.at(m))) {
                        now_entity.addRelatedObjectID(b_block_entity_list.at(m));
                    }
                }
            }
        }
    }    

    //再计算优先级为2
    for(i = 0; i < map_block_list.size(); i++) {
        MapRegion a_block = map_block_list.at(i);
        //获取到当前地图块内所有的实体对象，优先级为2（**此处应给一个获取地图块内不同优先级实体的方法**）
        vector<BaseUnit> a_block_entity_list = a_block.getRelatedMapUnitIDs();
        vector<int> related_map_block_list_0 = a_block.getRelatedMapUnitIDs0();
        vector<int> related_map_block_list_1 = a_block.getRelatedMapUnitIDs1();
        for (j = 0; j < related_map_block_list_0.size(); j++) {
            MapRegion b_block = //获取到related_map_block_list_0中的一个地图块类
            //获取到当前地图块内所有的实体对象（所有优先级的对象）
            vector<BaseUnit> b_block_entity_list = b_block.getIncludeObjectIDs();

            for (k = 0; k < a_block_entity_list.size(); k++) {
                BaseUnit now_entity = a_block_entity_list.at(k);
                //因为此处的优先级为2，2s更新一次，而时间片的大小为1s，因此使用取模运算
                if ((int)getTimeStamp() % 2 == 0) {
                    for (m = 0; m < b_block_entity_list.size(); m++) {
                        now_entity.addRelatedObjectID(b_block_entity_list.at(m));
                    }
                }
            }
        }

        //计算次最近相邻的地图块
        for (j = 0; j < related_map_block_list_1.size(); j++) {
            MapRegion b_block = //获取到related_map_block_list_1中的一个地图块类
            //获取到当前地图块内所有的实体对象（所有优先级的对象）
            vector<BaseUnit> b_block_entity_list = b_block.getIncludeObjectIDs();

            for (k = 0; k < a_block_entity_list.size(); k++) {
                FlightUnit now_entity = a_block_entity_list.at(k);
                //因为此处的优先级为2，2s更新一次，而时间片的大小为1s，因此使用取模运算
                if ((int)getTimeStamp() % 2 == 0) {
                    for (m = 0; m < b_block_entity_list.size(); m++) {
                        if (connected(now_entity, b_block_entity_list.at(m))) {
                            now_entity.addRelatedObjectID(b_block_entity_list.at(m));
                        }
                    }
                }
            }
        }
    }

    //再计算优先级为3
    for(i = 0; i < map_block_list.size(); i++) {
        MapRegion a_block = map_block_list.at(i);
        //获取到当前地图块内所有的实体对象，优先级为2（**此处应给一个获取地图块内不同优先级实体的方法**）
        vector<BaseUnit> a_block_entity_list = a_block.getRelatedMapUnitIDs();
        vector<int> related_map_block_list_0 = a_block.getRelatedMapUnitIDs0();
        vector<int> related_map_block_list_1 = a_block.getRelatedMapUnitIDs1();
        for (j = 0; j < related_map_block_list_0.size(); j++) {
            MapRegion b_block = //获取到related_map_block_list_0中的一个地图块类
            //获取到当前地图块内所有的实体对象（所有优先级的对象）
            vector<BaseUnit> b_block_entity_list = b_block.getIncludeObjectIDs();

            for (k = 0; k < a_block_entity_list.size(); k++) {
                BaseUnit now_entity = a_block_entity_list.at(k);
                //因为此处的优先级为3，5s更新一次，而时间片的大小为1s，因此使用取模运算
                if ((int)getTimeStamp() % 5 == 0) {
                    for (m = 0; m < b_block_entity_list.size(); m++) {
                        now_entity.addRelatedObjectID(b_block_entity_list.at(m));
                    }
                }
            }
        }

        //计算次最近相邻的地图块
        for (j = 0; j < related_map_block_list_1.size(); j++) {
            MapRegion b_block = //获取到related_map_block_list_1中的一个地图块类
            //获取到当前地图块内所有的实体对象（所有优先级的对象）
            vector<BaseUnit> b_block_entity_list = b_block.getIncludeObjectIDs();

            for (k = 0; k < a_block_entity_list.size(); k++) {
                FlightUnit now_entity = a_block_entity_list.at(k);
                //因为此处的优先级为3，5s更新一次，而时间片的大小为1s，因此使用取模运算
                if ((int)getTimeStamp() % 5 == 0) {
                    for (m = 0; m < b_block_entity_list.size(); m++) {
                        if (connected(now_entity, b_block_entity_list.at(m))) {
                            now_entity.addRelatedObjectID(b_block_entity_list.at(m));
                        }
                    }
                }
            }
        }
    }
}

void saveTrace()
{
    int i = 0;
    for (i = 0; i < entity_list.size(); i++) {
        //printInfo方法是类内部将自身位置、速度、方向等trace文件需要的信息以统一格式打包为一个字符串的方法
        fprintf(xxx.csv, (/*此处应进行类型转换*/)entity_list.at(i).printInfo());
    }
}

void printTrace()
{

}
