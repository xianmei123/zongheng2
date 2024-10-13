#include "UniqueIDGenerator.h"
#include <unordered_set>
#include <random>

int UniqueIDGenerator::generateUID() {
    // 随机数引擎，用于生成随机的ID
    std::random_device rd;
    std::mt19937 gen(rd());

    // 已生成的ID集合，用于检查是否重复
    std::unordered_set<int> generatedIDs;

    // ID的范围，根据需要进行调整
    const int minID = 1;
    const int maxID = 10000000;

    std::uniform_int_distribution<> dis(minID, maxID);

    int uid = dis(gen); // 生成随机ID

    // 确保生成的ID是唯一的
    while (generatedIDs.find(uid) != generatedIDs.end()) {
        uid = dis(gen); // 重新生成ID
    }

    generatedIDs.insert(uid); // 将生成的ID添加到集合中
    return uid;
}
