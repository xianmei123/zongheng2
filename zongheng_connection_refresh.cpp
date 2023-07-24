/*please include some header files*/
#include <iostream>
#include <FlightUnit.h>
#include <NonMovableUnit.h>
#include <MapRegion.h>
#include <GroundUnit.h>

using namespace std;

//暂时使用FlightUnit作为单位，后续应使用基类
vector<FlightUnit> flight_entity_list;
vector<GroundUnit> ground_entity_list;
vector<NonMovableUnit> nonmove_entity_list;

//MapRegion作为地图块类
vector<MapRegion> map_block_list;



/*@required 所有实体的列表(暂时命名为entity_list)，所有地图块的列表(暂时命名为map_block_list)*/
/*@process 遍历所有的实体，根据实体类提供的方法判断该实体属于哪一块地图块，并将其加入地图块的关联列表中*/
/*@result 所有实体根据其最新位置，已加入到相应的地图块中的关联列表*/
/*@output null*/
void refresh_map_block_list()
{
    int i = 0;
    for (i = 0; i < entity_list.size(); i++) {
        //getMapBlockIndex方法可以作为基类的一个方法，在类内部根据类的位置信息去计算返回当前实体所处的地图号序号。
        int map_block_index = (entity_list.at(i)).getMapBlockIndex();
        //应注意此处将一个实体加入某一块地图时，应同时删除该实体之前所处于的地图块的关联列表中的相应项
        (map_block_list.at(map_block_index)).addEntity(entity_list.at(i));
    }
}

//该方法利用地图块顺序的遍历方式进行更新计算，还可以考虑利用实体遍历的方式计算（n^2）
/*@required 所有地图块的列表(暂时命名为map_block_list)*/
/*@process 先按地图块的ID从小到大遍历，将当前地图块中的实体与其中的列表0和列表1的地图块内的实体进行通讯连接计算和更新，按照优先级的顺序进行*/
/*@result 每个实体类内部的relatedObjectIDs列表根据当前位置更新为最新的内容*/
/*@output null*/
void refresh_entity_connection()
{
    int i = 0;
    int j = 0;
    int k = 0;
    //优先计算优先级为1
    for(i = 0; i < map_block_list.size(); i++) {
        MapRegion a_block = map_block_list.at(i);
        vector<FlightUnit> = a_block.getRelatedMapUnitIDs();
        vector<int> related_map_block_list_0 = a_block.getRelatedMapUnitIDs0();
        vector<int> related_map_block_list_1 = a_block.getRelatedMapUnitIDs1();
        for (j = 0; j < related_map_block_list_0.size(); j++) {
            MapRegion b_block = //获取到related_map_block_list_0中的一个地图块类
            for (k = 0; k < b_block.get)
        }
    }    
}
